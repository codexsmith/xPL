/* xPL-io.c - xPL I/O handling */
/* Copyright 2004 (c), Gerald R Duprey Jr */

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include <termios.h>

#include "xPL.h"
#include "xPL_priv.h"

#define POLL_GROW_BY 32

/* User defined data for each poll entry */
typedef struct _pollUserInfo {
  xPL_IOHandler ioHandler;
  int userValue;
} _pollUserInfo, *pollUserInfoPtr;

/* Just to make things a bit cleaner */
typedef struct pollfd * pollInfoPtr;

/* Info on poll space usage */
static int pollInfoSize  = 0;     /* # of allocated slots in pollInfo */
static int pollInfoCount = 0;     /* # of slots in use in pollInfo */
static pollInfoPtr pollInfo = NULL;
static pollUserInfoPtr pollUserInfo = NULL;

/* Timeout support structures */
typedef struct {
  int timeoutInterval;
  time_t lastTimedoutAt;
  xPL_TimeoutHandler timeoutHandler;
  xPL_ObjectPtr userValue;
} TimeoutHandler, *TimeoutHandlerPtr;

#define GROW_TIMEOUT_LIST_BY 4
static int timeoutCount = 0;
static int timeoutAllocCount = 0;
static TimeoutHandler *timeoutList = NULL;
static time_t timeoutChecksLastDoneAt = -1;

/* The xPL FD */
int xPLFD = -1;
pid_t xPLPID;

/* Connection Info */
static xPL_ConnectType xPLconnectType = xcViaHub;  /* Default is via hub */
static int xPLPort = BASE_XPL_PORT;                /* Current actual port */

static int xPLBroadcastFD = -1;                    /* FD for broadcast socket */
static char xPLInterfaceName[64] = "";             /* Interface to connect to (empty=auto assign) */
static struct in_addr xPLInterfaceAddr;            /* Address associated with interface */
static struct sockaddr_in xPLBroadcastAddr;        /* Broadcasing address */

static time_t heartbeatCheckLastDoneAt = -1;

/* Status/state */
static Bool xPL_IODeviceInstalled = FALSE;

/* Conversion buffers */
static char uniqueIDPrefix[9] = "";
static char base36Table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
			      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			      'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			      'u', 'v', 'w', 'x', 'y', 'z' };


/* Convert a long value into an 8 digit base36 number */
/* and concatenate it onto the passed string          */
static void longToBase32(unsigned long theValue, String theBuffer) {
  int charPtr, buffLen;

  /* Fill with zeros */
  strcat(theBuffer, "00000000");
  buffLen = strlen(theBuffer);

  /* Handle the simple case */
  if (theValue == 0) return;

  for(charPtr = buffLen - 1; charPtr >= (buffLen - 8); charPtr--) {
    theBuffer[charPtr] = base36Table[theValue % 36];
    if (theValue < 36) break;
    theValue = theValue / 36;
  }
}

/* Create a reasonably uniqe 16 character identifier */    
String xPL_getFairlyUniqueIdent() {
  char newIdent[32];
  uint32_t localAddr;
  struct timeval rightNow;
  unsigned long timeInMillis;

  /* First time around, format the prefix ident */
  if (*uniqueIDPrefix == '\0') {
    localAddr = htonl(xPLInterfaceAddr.s_addr);
    strcpy(uniqueIDPrefix, xPL_intToHex((localAddr >> 24) & 255));
    strcat(uniqueIDPrefix, xPL_intToHex((localAddr >> 16) & 255));
    strcat(uniqueIDPrefix, xPL_intToHex((localAddr >> 8) & 255));
    strcat(uniqueIDPrefix, xPL_intToHex(localAddr & 255));
  }

  /* Now tack on the time of day, radix-32 encoded (which allows   */
  /* packing in a lot more uniqueness for the 8 characters we have */
  gettimeofday(&rightNow, NULL);
  timeInMillis = (rightNow.tv_sec * 1000) + (rightNow.tv_usec / 1000);
  strcpy(newIdent, uniqueIDPrefix);
  longToBase32(timeInMillis, newIdent);
  if (strlen(newIdent) > 16) newIdent[16] = '\0';

  /* Pass a copy off */
  return xPL_StrDup(newIdent);
}


/* Add an IO channel to monitor/dispatch to.  theFD is the FD that is */
/* open and should be monitored.  ioHandler is the routine that is    */
/* is called when there is activity on the channel.  userValue is an  */
/* integer passed directly to the ioHandler, frequently used to track */
/* context information. watchRead, watchWrite, watchError tell what   */
/* sort of things need to be monitored.                               */
Bool xPL_addIODevice(xPL_IOHandler theIOHandler, int userValue, int theFD, Bool watchRead, Bool watchWrite, Bool watchError) {
  /* Make sure they are going to really do something */
  if (!watchRead && !watchWrite && !watchError) return FALSE;

  /* See if we need to allocate more space */
  if (pollInfoCount == pollInfoSize) {
    pollInfoSize += POLL_GROW_BY;
    pollInfo = (pollInfoPtr) realloc(pollInfo, sizeof(struct pollfd) * pollInfoSize);
    pollUserInfo = (pollUserInfoPtr) realloc(pollUserInfo, sizeof(struct _pollUserInfo) * pollInfoSize);
  }

  /* Install this */
  pollInfo[pollInfoCount].fd = theFD;
  pollInfo[pollInfoCount].events = 0;
  if (watchRead) pollInfo[pollInfoCount].events |= POLLIN;
  if (watchWrite) pollInfo[pollInfoCount].events |= POLLOUT;
  if (watchError) pollInfo[pollInfoCount].events |= POLLERR;
  pollUserInfo[pollInfoCount].ioHandler = theIOHandler;
  pollUserInfo[pollInfoCount].userValue = userValue;
  pollInfoCount++;

  xPL_Debug("Added managed IO device, now %d devices", pollInfoCount);
  return TRUE;
}

/* Remove an IO channel based on the passed fd.  If the */
/* fd exists, it's removed and TRUE is returned.  If    */
/* the fd doesn't exist, FALSE is returned.             */
Bool xPL_removeIODevice(int theFD) {
  int infoIndex;

  /* Find the fd in the list */
  for (infoIndex = 0; infoIndex < pollInfoCount; infoIndex++) {
    /* Skip if this isn't the fd we care about */
    if (pollInfo[infoIndex].fd != theFD) continue;

    /* Remove the item and shuffle everything down */
    pollInfoCount--;
    if (infoIndex < pollInfoCount) {
      memcpy(&pollInfo[infoIndex], &pollInfo[infoIndex + 1], sizeof(struct pollfd) * (pollInfoCount - infoIndex));
      memcpy(&pollUserInfo[infoIndex], &pollUserInfo[infoIndex + 1], sizeof(struct _pollUserInfo) * (pollInfoCount - infoIndex));
    }

    xPL_Debug("Removed managed IO device, now %d devices", pollInfoCount);
    return TRUE;
  }

  /* Never found it */
  xPL_Debug("Unable to remove managed IO device, fd=%d not found", theFD);
  return FALSE;
}

/* Allocate a new timeout handler and install it into the list */
void xPL_addTimeoutHandler(xPL_TimeoutHandler timeoutHandler, int timeoutInSeconds, xPL_ObjectPtr userValue) {
  TimeoutHandlerPtr theHandler;

  /* Allocate a handler */
  if (timeoutCount == timeoutAllocCount) {
    timeoutAllocCount += GROW_TIMEOUT_LIST_BY;
    timeoutList = (TimeoutHandlerPtr) realloc(timeoutList, sizeof(TimeoutHandler) * timeoutAllocCount);
  }
  theHandler = &timeoutList[timeoutCount++];
  bzero(theHandler, sizeof(TimeoutHandler));

  /* Install values */
  theHandler->timeoutInterval = timeoutInSeconds;
  theHandler->timeoutHandler = timeoutHandler;
  theHandler->userValue = userValue;
  theHandler->lastTimedoutAt = time(NULL);
}

/* Remove a previously allocated timeout handler */
Bool xPL_removeTimeoutHandler(xPL_TimeoutHandler timeoutHandler) {
  int timeoutIndex;

  for (timeoutIndex = 0; timeoutIndex < timeoutCount; timeoutIndex++) {
    if (timeoutList[timeoutIndex].timeoutHandler != timeoutHandler) continue;

    /* Reduce count */
    timeoutCount--;
    if (timeoutIndex < timeoutCount) {
      memcpy(&(timeoutList[timeoutIndex]), &(timeoutList[timeoutIndex + 1]), (timeoutCount - timeoutIndex) * sizeof(TimeoutHandler));
    }
    
    return TRUE;
  }

  /* No Match */
  return FALSE;
}

/* Return the xPL FD */
int xPL_getFD() {
  return xPLFD;
}

/* Return the current connection type */
xPL_ConnectType xPL_getConnectType() {
  return xPLconnectType;
}

/* Get the connection port */
int xPL_getPort() {
  return xPLPort;
}

/* Return IP address */
String xPL_getBroadcastIPAddr() {
  return inet_ntoa(xPLBroadcastAddr.sin_addr);
}

/* Return listing IP address */
String xPL_getListenerIPAddr() {
  return inet_ntoa(xPLInterfaceAddr);
}

/* Get the xPL Interface */
String xPL_getBroadcastInterface() {
  return xPLInterfaceName;
}

/* Set the interface */
void xPL_setBroadcastInterface(String newInterfaceName) {
  /* Can't change the interface after it's open */
  if (xPLFD != -1) return;

  strcpy(xPLInterfaceName, newInterfaceName);
}

/* Make a fd non-blocking */
static Bool markNonblocking(int thefd) {
  int theValue;

  if ((theValue = fcntl(thefd, F_GETFL, 0)) != -1) {
    return (fcntl(thefd, F_SETFL, theValue|O_NONBLOCK) != -1);
  } else
    return FALSE;
}

/* Try to increase the receive buffer as big as possible.  if */
/* we make it bigger, return TRUE.  Otherwise, if no change, FALSE */
static Bool maximizeReceiveBufferSize(int thefd) {
  int startRcvBuffSize, idealRcvBuffSize, finalRcvBuffSize;
  socklen_t buffLen = sizeof(int);

  /* Get current receive buffer size */
  if (getsockopt(thefd, SOL_SOCKET, SO_RCVBUF, &startRcvBuffSize, &buffLen) != 0) 
    xPL_Debug("Unable to read receive socket buffer size - %s (%d)", strerror(errno), errno);
  else
    xPL_Debug("Initial receive socket buffer size is %d bytes", startRcvBuffSize);

  /* Try to increase the buffer (maybe multiple times) */
  for (idealRcvBuffSize = 1024000; idealRcvBuffSize > startRcvBuffSize; ) {
    /* Attempt to set the buffer size */
    if (setsockopt(thefd, SOL_SOCKET, SO_RCVBUF, &idealRcvBuffSize, sizeof(int)) != 0) {
      xPL_Debug("Not able to set receive buffer to %d bytes - retrying", idealRcvBuffSize);
      idealRcvBuffSize -= 64000;
      continue;
    }

    /* We did it!  Get the current size and bail out */
    buffLen = sizeof(int);
    if (getsockopt(thefd, SOL_SOCKET, SO_RCVBUF, &finalRcvBuffSize, &buffLen) != 0) 
      xPL_Debug("Unable to read receive socket buffer size - %s (%d)", strerror(errno), errno);
    else
      xPL_Debug("Actual receive socket buffer size is %d bytes", finalRcvBuffSize);

    return (finalRcvBuffSize > startRcvBuffSize);
  }

  /* We weren't able to increase it */
  xPL_Debug("Unable to increase receive buffer size - dang!");
  return FALSE;
}

/* Attempt to make a standalone connection */
static Bool attemptStandaloneConnection() {
  int sockfd;
  int flag = 1;
  struct protoent *ppe;
  struct sockaddr_in theSockInfo;

  /* Init the socket definition */
  bzero(&theSockInfo, sizeof(theSockInfo));
  theSockInfo.sin_family = AF_INET;
  theSockInfo.sin_addr.s_addr = INADDR_ANY;
  theSockInfo.sin_port = htons(xPLPort);

  /* Map protocol name */
  if ((ppe = getprotobyname("udp")) == 0) {
    xPL_Debug("Unable to lookup UDP protocol info");
    return FALSE;
  }

  /* Attempt to creat the socket */
  if ((sockfd = socket(PF_INET, SOCK_DGRAM, ppe->p_proto)) < 0) {
    xPL_Debug("Unable to create listener socket %s (%d)", strerror(errno), errno);
    return FALSE;
  }

  /* Allow re-use and restart */
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0) {
    xPL_Debug("Unable to set SO_REUSEADDR on socket %s (%d)", strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* Mark as a broadcast socket */
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0) {
    xPL_Debug("Unable to set SO_BROADCAST on socket %s (%d)", strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* Attempt to bind */
  if (bind(sockfd, (struct sockaddr *)&theSockInfo, sizeof(theSockInfo)) < 0) {
    xPL_Debug("Unable to bind listener socket to port %d, %s (%d)", xPLPort, strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* We are ready to go */
  xPLFD = sockfd;
  markNonblocking(xPLFD);
  maximizeReceiveBufferSize(xPLFD);
  xPL_Debug("xPL Started in standalone mode");
  return TRUE;
}

/* Attempt to make a hub based connection */
static Bool attempHubConnection() {
  int sockfd;
  int flag = 1;
  int sockSize = sizeof(struct sockaddr_in);
  struct protoent *ppe;
  struct sockaddr_in theSockInfo;

  /* Init the socket definition */
  bzero(&theSockInfo, sizeof(theSockInfo));
  theSockInfo.sin_family = AF_INET;
  theSockInfo.sin_addr.s_addr = INADDR_ANY;
  theSockInfo.sin_port = htons(0);

  /* Map protocol name */
  if ((ppe = getprotobyname("udp")) == 0) {
    xPL_Debug("Unable to lookup UDP protocol info");
    return FALSE;
  }

  /* Attempt to creat the socket */
  if ((sockfd = socket(PF_INET, SOCK_DGRAM, ppe->p_proto)) < 0) {
    xPL_Debug("Unable to create listener socket %s (%d)", strerror(errno), errno);
    return FALSE;
  }

  /* Mark as a broadcast socket */
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0) {
    xPL_Debug("Unable to set SO_BROADCAST on socket %s (%d)", strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* Attempt to bind */
  if ((bind(sockfd, (struct sockaddr *)&theSockInfo, sockSize)) < 0) {
    xPL_Debug("Unable to bind listener socket to port %d, %s (%d)", xPLPort, strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* Fetch the actual socket port # */
  if (getsockname(sockfd, (struct sockaddr *) &theSockInfo, (socklen_t *) &sockSize)) {
    xPL_Debug("Unable to fetch socket info for bound listener, %s (%d)", strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* We are ready to go */
  xPLFD = sockfd;
  xPLPort = ntohs(theSockInfo.sin_port);
  markNonblocking(xPLFD);
  maximizeReceiveBufferSize(xPLFD);
  xPL_Debug("xPL Starting in Hub mode on port %d", xPLPort);
  return TRUE;
}    

/* Figure out what sort of connection to make and do it */
static Bool makeConnection(xPL_ConnectType theConnectType) {
  /* Try an stand along connection */
  if ((theConnectType == xcStandAlone) || (theConnectType == xcAuto)) {
    /* Attempt the connection */
    xPL_Debug("Attemping standalone xPL");
    if (attemptStandaloneConnection()) {
      xPLconnectType = xcStandAlone;
      return TRUE;
    }
    
    /* If we failed and this what we want, bomb out */
    xPL_Debug("Standalong connect failed - %d %d", theConnectType, xcStandAlone);
    if (theConnectType == xcStandAlone) return FALSE;
  }

  /* Try a hub based connection */
  xPL_Debug("Attempting hub based xPL");
  if (!attempHubConnection()) return FALSE;
  xPLconnectType = xcViaHub;
  return TRUE;
}

/* When no interface is selected, scan the list of interface */
/* and choose the first one that looks OK.  If nothing is    */
/* found, then return FALSE.  Otherwise, install the name as */
/* the xPL interface and return TRUE                         */
static Bool findDefaultInterface(int sockfd) {
  char intBuff[1024];
  struct ifconf ifc;
  struct ifreq *ifr;
  struct ifreq interfaceInfo;
  int intIndex;

  /* Request list of intefaces */
  ifc.ifc_len = sizeof(intBuff); 
  ifc.ifc_buf = intBuff; 
  if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) { 
    return FALSE;
  }

  /* Try each interface until one works */
  ifr = ifc.ifc_req;
  for (intIndex = 0; intIndex < ifc.ifc_len / sizeof(struct ifreq); intIndex++) {
    /* Init the interface info request */
    bzero(&interfaceInfo, sizeof(struct ifreq));
    interfaceInfo.ifr_addr.sa_family = AF_INET; 
    strcpy(interfaceInfo.ifr_name, ifr[intIndex].ifr_name);

    /* Get device flags */
    if (ioctl(sockfd, SIOCGIFFLAGS, &interfaceInfo) != 0) continue;

    xPL_Debug("Checking if interface %s is valid w/flags %d", interfaceInfo.ifr_name, interfaceInfo.ifr_flags);

    /* Insure this interface is active and not loopback */
    if ((interfaceInfo.ifr_flags & IFF_UP) == 0) continue;
    if ((interfaceInfo.ifr_flags & IFF_LOOPBACK) != 0) continue;

    /* If successful, use this interface */
    strcpy(xPLInterfaceName, ifr[intIndex].ifr_name);
    xPL_Debug("Choose interface %s as default interface", xPLInterfaceName);
    return TRUE;
  }

  /* No good interface found */
  return FALSE;
}


/* Create a socket for broadcasting messages */
static Bool setupBroadcastAddr() {
  int sockfd;
  int flag = 1;
  struct protoent *ppe;
  struct ifreq interfaceInfo;
  struct in_addr interfaceNetmask;  
  
  /* Map protocol name */
  if ((ppe = getprotobyname("udp")) == 0) {
    xPL_Error("Unable to lookup UDP protocol info");
    return FALSE;
  }

  /* Attempt to create a socket */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, ppe->p_proto)) < 0) {
    xPL_Error("Unable to create broadcast socket %s (%d)", strerror(errno), errno);
    return FALSE;
  }

  /* Mark as a broadcasting socket */
  if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0) {
    xPL_Error("Unable to set SO_BROADCAST on socket %s (%d)", strerror(errno), errno);
    close(sockfd);
    return FALSE;
  }

  /* See if we need to find a default interface */
  if ((xPLInterfaceName == NULL) || (strlen(xPLInterfaceName) == 0)) {
    if (!findDefaultInterface(sockfd)) {
      xPL_Error("Could not find a working, non-loopback network interface");
      close(sockfd);
      return FALSE;
    }
  }

  /* Init the interface info request */
  bzero(&interfaceInfo, sizeof(struct ifreq));
  interfaceInfo.ifr_addr.sa_family = AF_INET; 
  strcpy(interfaceInfo.ifr_name, xPLInterfaceName);
  
  /* Get our interface address */
  if (ioctl(sockfd, SIOCGIFADDR, &interfaceInfo) != 0) {
    xPL_Error("Unable to get IP addr for interface %s", xPLInterfaceName);
    close(sockfd);
    return FALSE;
  }
  xPLInterfaceAddr.s_addr = ((struct sockaddr_in *) &interfaceInfo.ifr_addr)->sin_addr.s_addr;  
  xPL_Debug("Auto-assigning IP address of %s", inet_ntoa(xPLInterfaceAddr));

  /* Get interface netmask */
  bzero(&interfaceInfo, sizeof(struct ifreq));
  interfaceInfo.ifr_addr.sa_family = AF_INET; 
  interfaceInfo.ifr_broadaddr.sa_family = AF_INET;
  strcpy(interfaceInfo.ifr_name, xPLInterfaceName);
  if (ioctl(sockfd, SIOCGIFNETMASK, &interfaceInfo) != 0) {
    xPL_Error("Unable to extract the interface net mask");
    close(sockfd);
    return FALSE;
  }
  interfaceNetmask.s_addr = ((struct sockaddr_in *) &interfaceInfo.ifr_netmask)->sin_addr.s_addr;  

  /* Build our broadcast addr */
  bzero(&xPLBroadcastAddr, sizeof(xPLBroadcastAddr));
  xPLBroadcastAddr.sin_family = AF_INET;
  xPLBroadcastAddr.sin_addr.s_addr = xPLInterfaceAddr.s_addr | ~interfaceNetmask.s_addr;
  xPLBroadcastAddr.sin_port = htons(BASE_XPL_PORT);
  xPLBroadcastFD = sockfd;
  markNonblocking(sockfd);

  /* And we are done */
  xPL_Debug("Assigned xPL Broadcast address of %s, port %d", inet_ntoa(xPLBroadcastAddr.sin_addr), BASE_XPL_PORT);
  return TRUE;
}

/* Send the passed string and return TRUE if it appears it */
/* or FALSE if there was an error                          */
Bool xPL_sendRawMessage(String theData, int dataLen) {
  int bytesSent;

  /* Try to send the message */
  if ((bytesSent = sendto(xPLBroadcastFD, theData, dataLen, 0, 
			  (struct sockaddr *) &xPLBroadcastAddr, sizeof(struct sockaddr_in))) != dataLen) {
    xPL_Debug("Unable to broadcast message, %s (%d)", strerror(errno), errno);
    return FALSE;
  }
  xPL_Debug("Broadcasted %d bytes (of %d attempted)", bytesSent, dataLen);

  /* Okey dokey then */
  return TRUE;
}

/* Attempt to start the xPL library.  If we are already "started" */
/* then bail out                                                  */
Bool xPL_initialize(xPL_ConnectType theConnectType) {
  /* If running, we have nothing to do */
  if (xPLFD != -1) return FALSE;
  
  /* Setup the broadcasting address */
  if (!setupBroadcastAddr()) return FALSE;

  /* Attempt to make our connection */
  if (!makeConnection(theConnectType)) return FALSE;

  /* Install our pid */
  xPLPID = getpid();

  /* Install a listener for xPL oriented messages */
  if (!xPL_IODeviceInstalled) {
    if (xPL_addIODevice(xPL_receiveMessage, -1, xPLFD, TRUE, FALSE, FALSE))
      xPL_IODeviceInstalled = TRUE;
  }

  /* Add a message listener for services */
  xPL_addMessageListener(xPL_handleServiceMessage, NULL);

  /* We are ready to go */
  return TRUE;
}

/* Stop the xPL library.  If already stopped, bail.  Otherwise, */
/* we close our connection, release any/all resources and reset */
Bool xPL_shutdown() {
  /* If already stopped, bail */
  if (xPLFD == -1) return FALSE;

  /* Shutdown all services */
  xPL_disableAllServices();

  /* Remove xPL Listener */
  if (xPL_removeIODevice(xPLFD)) xPL_IODeviceInstalled = FALSE;

  /* Close the connection */
  close(xPLBroadcastFD); xPLBroadcastFD = -1;
  close(xPLFD);  xPLFD = -1;

  /** Other Stuff here **/

  /* And we are done */
  return TRUE;
}

/* Check for any registered timeouts */
static void checkAllTimeoutHandlers() {
  int timeoutIndex;
  int elapsedTime;
  time_t rightNow = time(NULL);

  for (timeoutIndex = timeoutCount - 1; timeoutIndex >= 0; timeoutIndex--) {
    elapsedTime = rightNow - timeoutList[timeoutIndex].lastTimedoutAt;
    if (elapsedTime >= timeoutList[timeoutIndex].timeoutInterval) {
      timeoutList[timeoutIndex].timeoutHandler(elapsedTime, timeoutList[timeoutIndex].userValue);
      timeoutList[timeoutIndex].lastTimedoutAt = rightNow;
    }
  }
}

/* Timeout processing - a-periodic processing work */
static void doTimeouts() {
  time_t rightNow = time(NULL);

  /* See if it's time to do heartbeats */
  if ((heartbeatCheckLastDoneAt == -1) || ((rightNow - heartbeatCheckLastDoneAt) >= 5) || !xPL_isHubConfirmed()) {
    /* Send Heartbeats out (if needed) */
    xPL_sendTimelyHeartbeats();
  
    /* Flag when we did this last */
    heartbeatCheckLastDoneAt = rightNow;
  }

  /* Do timeout checks */
  if ((timeoutChecksLastDoneAt == -1) || ((rightNow - timeoutChecksLastDoneAt) >= 1)) {
    checkAllTimeoutHandlers();
    timeoutChecksLastDoneAt = rightNow;
  }
}


/* Process xPL messages and I/O.  If theTimeout is 0, then we */
/* process any pending messages and then immediatly return.   */
/* If theTimeout > 0 then we process any pending messages,    */
/* waiting up to theTimeout milliseconds and then return.  If */
/* theTimeout is -1, then we process messages and wait and do */
/* not return until xPLLib is stopped.  In all cases, if at   */
/* lease one xPL message was processed during the duration of */
/* this call, TRUE is returned.  otherwise, FALSE.            */
Bool xPL_processMessages(int theTimeout) {
  Bool xPLMessageProcessed = FALSE;
  Bool timeoutDone = FALSE;
  int activeDevices, deviceIndex;
  int thisTimeout;
  int timeoutsSoFar = 0;

  for(;xPLFD != -1;) {
    /* Compute the timeout */
    if (theTimeout > 0) {
      /* If the timeout is more than once per second, we cap this to */
      /* once per second and track how long we've waited to make up  */
      if (theTimeout > 1000) {
	if ((timeoutsSoFar + 1000) > theTimeout) 
	  thisTimeout = theTimeout - timeoutsSoFar;
	else
	  thisTimeout = 1000;
      } else
	thisTimeout = theTimeout;
    } else if (theTimeout == -1) 
      thisTimeout = 1000;
    else
      thisTimeout = 0;
      
    /* Poll for an active device */
    SYSCALL(activeDevices = poll(pollInfo, pollInfoCount, thisTimeout));
    if (xPLFD == -1) break;

    /* Handle errors */
    if (activeDevices == -1) {
      /* Andything else is an error */
      xPL_Debug("Error while polling devices - %s (%d) - terminating", strerror(errno), errno);
      return xPLMessageProcessed;
    }

    /* Handle a timeout */
    if (activeDevices == 0) {
      /* If the timer was <1000, then bail out */
      if (thisTimeout < 1000) break;

      /* Service timeouts */
      doTimeouts();
      timeoutDone = TRUE;
      timeoutsSoFar += thisTimeout;
      continue;
    }

    /* Clear timeouts so far */
    timeoutsSoFar = 0;

    /* Find devices that have something to say and dispatch to them */
    for (deviceIndex = pollInfoCount - 1; deviceIndex >= 0; deviceIndex--) {
      /* Skip unless something happened */
      if (pollInfo[deviceIndex].revents == 0) continue;

      /* Dispatch to IO handler */
      pollUserInfo[deviceIndex].ioHandler(pollInfo[deviceIndex].fd, pollInfo[deviceIndex].revents, pollUserInfo[deviceIndex].userValue);

      /* See if the xPLLib is stopped */
      if (xPLFD == -1) break;

      /* Knock down our active device count.  If we hit zero, we've */
      /* processed all known active devices this time around.       */
      if (!(--activeDevices)) break;
    }
  }

  /* Return final status */
  if (!timeoutDone) doTimeouts();
  return xPLMessageProcessed;
}
