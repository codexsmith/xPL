/* xPL.h - xPL Public API */
/* Copyright 2004 (c), Gerald R Duprey Jr */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#define XPLLIB_VERSION "V1.3a"

#define BASE_XPL_PORT 3865

#define DEFAULT_HEARTBEAT_INTERVAL 300
#define CONFIG_HEARTBEAT_INTERVAL 60
#define HUB_DISCOVERY_INTERVAL 3

/* Some helpful types */
#ifndef COMMON_TYPES
typedef enum { FALSE, TRUE }  Bool;
typedef char * String;

#define COMMON_TYPES
#endif

/* Some helpful constants */
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef INADDR_NONE
  #define INADDR_NONE 0xffffffff
#endif

#define SYSCALL(call) while(((call) == -1) && (errno == EINTR))

/* xPL Connection mode */
typedef enum {xcStandAlone, xcViaHub, xcAuto} xPL_ConnectType;

/* Pointer to nothing in particular */
typedef void * xPL_ObjectPtr;

/* A discrete name/value structure */
typedef struct {
  String itemName;
  String itemValue;
  Bool isBinary;
  int binaryLength;
} xPL_NameValuePair, *xPL_NameValuePairPtr;

/* A list of name/value pairs */
typedef struct {
  int namedValueCount;
  int namedValueAlloc;
  xPL_NameValuePairPtr *namedValues;
} xPL_NameValueList, *xPL_NameValueListPtr;

/* Possible xPL message types */
typedef enum { xPL_MESSAGE_ANY, xPL_MESSAGE_COMMAND, xPL_MESSAGE_STATUS, xPL_MESSAGE_TRIGGER } xPL_MessageType;

/* Describe a received message */
typedef struct {
  xPL_MessageType messageType;
  int hopCount;
  Bool receivedMessage; /* TRUE if received, FALSE if being sent */

  String sourceVendor;
  String sourceDeviceID;
  String sourceInstanceID;

  Bool isGroupMessage;
  String groupName;

  Bool isBroadcastMessage;
  String targetVendor;
  String targetDeviceID;
  String targetInstanceID;

  String schemaClass;
  String schemaType;

  xPL_NameValueListPtr messageBody;
} xPL_Message, *xPL_MessagePtr;

typedef struct {
  xPL_MessageType matchOnMessageType;
  String matchOnVendor;
  String matchOnDeviceID;
  String matchOnInstanceID;
  String matchOnSchemaClass;
  String matchOnSchemaType;
} xPL_ServiceFilter, *xPL_ServiceFilterPtr;

typedef enum { xPL_CONFIG_OPTIONAL, xPL_CONFIG_MANDATORY, xPL_CONFIG_RECONF } xPL_ConfigurableType;

typedef struct {
  String itemName;
  xPL_ConfigurableType itemType;
  int maxValueCount;

  int valueCount;
  int valueAllocCount;
  String *valueList;
} xPL_ServiceConfigurable, *xPL_ServiceConfigurablePtr;


typedef struct _xPL_Service xPL_Service;
typedef struct _xPL_Service *xPL_ServicePtr;

/** Changes to a services configuration **/
typedef void (* xPL_ServiceConfigChangedListener) (xPL_ServicePtr, xPL_ObjectPtr);

/** Service Listener Support **/
typedef void (* xPL_ServiceListener)(xPL_ServicePtr, xPL_MessagePtr, xPL_ObjectPtr);

typedef struct {
  xPL_MessageType matchMessageType;
  String matchSchemaClass;
  String matchSchemaType;
  xPL_ObjectPtr userValue;
  xPL_ServiceListener serviceListener;
} xPL_ServiceListenerDef, *xPL_ServiceListenerDefPtr;

typedef struct {
  xPL_ServiceConfigChangedListener changeListener;
  xPL_ObjectPtr userValue;
} xPL_ServiceChangedListenerDef, *xPL_ServiceChangedListenerDefPtr;

/* Describe a xPL service */
struct _xPL_Service {
  Bool serviceEnabled;

  String serviceVendor;
  String serviceDeviceID;
  String serviceInstanceID;

  String serviceVersion;

  int groupCount;
  int groupAllocCount;
  String *groupList;

  Bool ignoreBroadcasts;

  int heartbeatInterval;
  time_t lastHeartbeatAt;
  xPL_MessagePtr heartbeatMessage;

  Bool configurableService;
  Bool serviceConfigured;
  String configFileName;
  int configChangedCount;
  int configChangedAllocCount;
  xPL_ServiceChangedListenerDefPtr changedListenerList;

  int configCount;
  int configAllocCount;
  xPL_ServiceConfigurablePtr configList;

  int filterCount;
  int filterAllocCount;
  xPL_ServiceFilterPtr messageFilterList;

  Bool reportOwnMessages;
  int listenerCount;
  int listenerAllocCount;
  xPL_ServiceListenerDefPtr serviceListenerList; 
};

/* xPL Service Support */
extern xPL_ServicePtr xPL_createService(String, String, String);
extern void xPL_releaseService(xPL_ServicePtr);

extern void xPL_setServiceEnabled(xPL_ServicePtr, Bool);
extern Bool xPL_isServiceEnabled(xPL_ServicePtr);

extern void xPL_setServiceVendor(xPL_ServicePtr, String);
extern String xPL_getServiceVendor(xPL_ServicePtr);

extern void xPL_setServiceDeviceID(xPL_ServicePtr, String);
extern String xPL_getServiceDeviceID(xPL_ServicePtr);

extern void xPL_setServiceInstanceID(xPL_ServicePtr, String);
extern String xPL_getServiceInstanceID(xPL_ServicePtr);

extern void xPL_setRespondingToBroadcasts(xPL_ServicePtr, Bool);
extern Bool xPL_isRespondingToBroadcasts(xPL_ServicePtr);

extern void xPL_setServiceVersion(xPL_ServicePtr, String);
extern String xPL_getServiceVersion(xPL_ServicePtr);

extern void xPL_setReportOwnMessages(xPL_ServicePtr, Bool);
extern Bool xPL_isReportOwnMessages(xPL_ServicePtr);

extern Bool xPL_isServiceFiltered(xPL_ServicePtr);
extern void xPL_clearServiceFilters(xPL_ServicePtr);

extern Bool xPL_doesServiceHaveGroups(xPL_ServicePtr);
extern void xPL_clearServiceGroups(xPL_ServicePtr);

extern void xPL_setHeartbeatInterval(xPL_ServicePtr, int);
extern int xPL_getHeartbeatInterval(xPL_ServicePtr);
extern void xPL_setTimelyHeartbeats();

extern Bool xPL_sendServiceMessage(xPL_ServicePtr, xPL_MessagePtr);

extern int xPL_getServiceCount();
extern xPL_ServicePtr xPL_getServiceAt(int);

/* xPL Service Configuratuion Support */
extern xPL_ServicePtr xPL_createConfigurableService(String, String, String);

extern Bool xPL_isConfigurableService(xPL_ServicePtr);
extern Bool xPL_isServiceConfigured(xPL_ServicePtr);

extern String xPL_getServiceConfigFile(xPL_ServicePtr);

extern Bool xPL_addServiceConfigurable(xPL_ServicePtr, String, xPL_ConfigurableType, int);
extern Bool xPL_removeServiceConfigurable(xPL_ServicePtr, String);
extern void xPL_removeAllServiceConfigurables(xPL_ServicePtr);

extern void xPL_clearServiceConfigValues(xPL_ServicePtr, String);
extern void xPL_clearAllServiceConfigValues(xPL_ServicePtr);

extern Bool xPL_addServiceConfigValue(xPL_ServicePtr, String, String);
extern void xPL_setServiceConfigValueAt(xPL_ServicePtr, String, int, String);
extern void xPL_setServiceConfigValue(xPL_ServicePtr, String, String);

extern int xPL_getServiceConfigValueCount(xPL_ServicePtr, String);
extern String xPL_getServiceConfigValueAt(xPL_ServicePtr, String, int);
extern String xPL_getServiceConfigValue(xPL_ServicePtr, String);

extern void xPL_addServiceConfigChangedListener(xPL_ServicePtr, xPL_ServiceConfigChangedListener, xPL_ObjectPtr);
extern Bool xPL_removeServiceConfigChangedListener(xPL_ServicePtr, xPL_ServiceConfigChangedListener);


/* Message support */

/* Accessors for messages (getters/setters).  Remember that you must NOT */
/* modify the returned value of an getter and that all setter values are */
/* copied off (i.e. future changes to the passed parameter will not be */
/* reflected in the message).  Finally, all string values are convered to */
/* upper case, except where noted.                                        */
extern void xPL_setMessageType(xPL_MessagePtr, xPL_MessageType);
extern xPL_MessageType xPL_getMessageType(xPL_MessagePtr);

extern int xPL_getHopCount(xPL_MessagePtr);
extern Bool xPL_isReceivedMessage(xPL_MessagePtr);

extern void xPL_setBroadcastMessage(xPL_MessagePtr, Bool);
extern Bool xPL_isBroadcastMessage(xPL_MessagePtr);

extern void xPL_setTargetGroup(xPL_MessagePtr, String);
extern String xPL_getTargetGroup(xPL_MessagePtr);
extern Bool xPL_isGroupMessage(xPL_MessagePtr);

extern void xPL_setTargetVendor(xPL_MessagePtr, String);
extern String xPL_getTargetVendor(xPL_MessagePtr);

extern void xPL_setTargetDeviceID(xPL_MessagePtr, String);
extern String xPL_getTargetDeviceID(xPL_MessagePtr);

extern void xPL_setTargetInstanceID(xPL_MessagePtr, String);
extern String xPL_getTargetInstanceID(xPL_MessagePtr);

extern void xPL_setTarget(xPL_MessagePtr, String, String, String);

extern void xPL_setSourceVendor(xPL_MessagePtr, String);
extern String xPL_getSourceVendor(xPL_MessagePtr);

extern void xPL_setSourceDeviceID(xPL_MessagePtr, String);
extern String xPL_getSourceDeviceID(xPL_MessagePtr);

extern void xPL_setSourceInstanceID(xPL_MessagePtr, String);
extern String xPL_getSourceInstanceID(xPL_MessagePtr);

extern void xPL_setSource(xPL_MessagePtr, String, String, String);

extern void xPL_setSchemaClass(xPL_MessagePtr, String);
extern String xPL_getSchemaClass(xPL_MessagePtr);
extern void xPL_setSchemaType(xPL_MessagePtr, String);
extern String xPL_getSchemaType(xPL_MessagePtr);
extern void xPL_setSchema(xPL_MessagePtr, String, String);

extern xPL_NameValueListPtr xPL_getMessageBody(xPL_MessagePtr);
extern Bool xPL_doesMessageNamedValueExist(xPL_MessagePtr, String);
extern String xPL_getMessageNamedValue(xPL_MessagePtr, String);

extern void xPL_clearMessageNamedValues(xPL_MessagePtr);
extern void xPL_addMessageNamedValue(xPL_MessagePtr, String, String);
extern void xPL_setMessageNamedValue(xPL_MessagePtr, String, String);
extern void xPL_setMessageNamedValues(xPL_MessagePtr, ...);

extern xPL_MessagePtr xPL_createTargetedMessage(xPL_ServicePtr, xPL_MessageType, String, String, String);
extern xPL_MessagePtr xPL_createGroupTargetedMessage(xPL_ServicePtr, xPL_MessageType, String);
extern xPL_MessagePtr xPL_createBroadcastMessage(xPL_ServicePtr, xPL_MessageType);
extern void xPL_releaseMessage(xPL_MessagePtr);

extern Bool xPL_sendMessage(xPL_MessagePtr);

extern void xPL_addServiceListener(xPL_ServicePtr, xPL_ServiceListener, xPL_MessageType, String, String, xPL_ObjectPtr);
extern Bool xPL_removeServiceListener(xPL_ServicePtr, xPL_ServiceListener);

/* General Library support */
extern void xPL_setDebugging(Bool);
extern Bool xPL_isDebugging();
extern void xPL_Debug(String, ...);
extern void xPL_Error(String, ...);

extern Bool xPL_initialize(xPL_ConnectType);
extern Bool xPL_shutdown();

extern int xPL_getFD();
extern int xPL_getPort();
extern xPL_ConnectType xPL_getConnectType();

extern Bool xPL_isHubConfirmed();

extern String xPL_formatMessage(xPL_MessagePtr);

extern String xPL_getBroadcastInterface();
extern void xPL_setBroadcastInterface(String);
extern String xPL_getBroadcastIPAddr();

extern String xPL_getListenerIPAddr();

extern Bool xPL_processMessages(int);

/* Event handler for user-registered I/O management */
typedef void (* xPL_IOHandler)(int, int, int);
extern Bool xPL_addIODevice(xPL_IOHandler, int, int, Bool, Bool, Bool);
extern Bool xPL_removeIODevice(int);

/* Event management of user timeouts */
typedef void (* xPL_TimeoutHandler)(int, xPL_ObjectPtr);
extern void xPL_addTimeoutHandler(xPL_TimeoutHandler, int, xPL_ObjectPtr);
extern Bool xPL_removeTimeoutHandler(xPL_TimeoutHandler);


/* General utility/helpers */
extern int xPL_strcmpIgnoreCase(String, String);
extern int xPL_strncmpIgnoreCase(String, String, int);
extern void xPL_Upcase(String);

extern String xPL_intToHex(int);
extern Bool xPL_hexToInt(String, int *);
extern String xPL_intToStr(int);
extern Bool xPL_strToInt(String, int *);

extern Bool xPL_parseCommonArgs(int *, String[], Bool);
extern xPL_ConnectType xPL_getParsedConnectionType();

/* Name/value list support */

/* Add a new named value to the list */
extern void xPL_addNamedValue(xPL_NameValueListPtr, String, String);

/* Update an existing named value or, if the name does not exist, add a new name/value */
extern void xPL_setNamedValue(xPL_NameValueListPtr, String, String);

/* Set a series of name/value pairs.  Each name must be followed by a value */
/* even if that value is NULL.                                              */
extern void xPL_setNamedValues(xPL_NameValueListPtr, ...);

/* Return # of name/value pairs in this list */
extern int xPL_getNamedValueCount(xPL_NameValueListPtr);

/* Return a Name/Value pair at the passed index (or NULL for a bad index) */
extern xPL_NameValuePairPtr xPL_getNamedValuePairAt(xPL_NameValueListPtr, int);

/* Given the passed list and name, return the index into the list of the name or -1 */
extern int xPL_getNamedValueIndex(xPL_NameValueListPtr, String);

/* Given the passed list and name, return the first matching name/value pair or NULL */
extern xPL_NameValuePairPtr xPL_getNamedValuePair(xPL_NameValueListPtr, String);

/* Given the passed list and name, return the first matching value or NULL */
/* Note: Value can be NULL because it was not found or because the actual */
/* value of the name/value pair is NULL.  If this matters, use doesNamedValueExist() */
extern String xPL_getNamedValue(xPL_NameValueListPtr, String);

/* Given the passed list and name, return TRUE if the named value exists in the list */
extern Bool xPL_doesNamedValueExist(xPL_NameValueListPtr, String);

/* Remove name specified by the passed index from the passed list */
extern void xPL_clearNamedValueAt(xPL_NameValueListPtr, int);

/* Remove all isntances of the passed name from the passed list */
extern void xPL_clearNamedValue(xPL_NameValueListPtr, String);

/* Remove All name/value pairs from the passed list */
extern void xPL_clearAllNamedValues(xPL_NameValueListPtr);

/** Raw Listener Support **/
typedef void (* xPL_rawListener)(String, int, xPL_ObjectPtr);
extern void xPL_addRawListener(xPL_rawListener, xPL_ObjectPtr);
extern Bool xPL_removeRawListener(xPL_rawListener);

/** Message Listener Support **/
typedef void (* xPL_messageListener)(xPL_MessagePtr, xPL_ObjectPtr);
extern void xPL_addMessageListener(xPL_messageListener, xPL_ObjectPtr);
extern Bool xPL_removeMessageListener(xPL_messageListener);

/** Built-in hub support **/
/* NOTE: The xPLLib must be initialize in xcStandalone mode */
/* in order to use a hub.                                   */
extern Bool xPL_startHub();
extern void xPL_stopHub();
extern Bool xPL_isHubRunning();

