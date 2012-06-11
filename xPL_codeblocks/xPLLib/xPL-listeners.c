/* xPL-listeners.c - handle class/type listeners */
/* Copyright 2004 (c), Gerald R Duprey Jr */

#include "xPL.h"
#include "xPL_priv.h"
#include <stdarg.h>

#define GROW_LIST_BY 16

typedef struct {
  xPL_messageListener theListener;
  xPL_ObjectPtr userValue;
} messageListener, *messageListenerPtr;

static int messageListenerCount = 0;
static int messageAllocListenerCount = 0;
static messageListenerPtr messageListenerList = NULL;

typedef struct {
  xPL_rawListener theListener;
  xPL_ObjectPtr userValue;
} rawListener, *rawListenerPtr;

static int rawListenerCount = 0;
static int rawAllocListenerCount = 0;
static rawListenerPtr rawListenerList = NULL;

/* Add a raw listener */
void xPL_addRawListener(xPL_rawListener theHandler, xPL_ObjectPtr userValue) {
  rawListenerPtr theListener;

  /* See if there is a slot to install and allocate more if not */
  if (rawListenerCount == rawAllocListenerCount) {
    rawAllocListenerCount += GROW_LIST_BY;
    rawListenerList = realloc(rawListenerList, sizeof(rawListener) * rawAllocListenerCount);
  }
  theListener = &rawListenerList[rawListenerCount++];
  bzero(theListener, sizeof(rawListener));

  /* Install values */
  theListener->theListener = theHandler;
  theListener->userValue = userValue;
}

/* Remove a raw listener */
Bool xPL_removeRawListener(xPL_rawListener theHandler) {
  int listenerIndex;
  rawListenerPtr theListener;

  for (listenerIndex = 0; listenerIndex < rawListenerCount; listenerIndex++) {
    theListener = &rawListenerList[listenerIndex];
    if (theListener->theListener != theHandler) continue;
    
    /* Remove it from the listener list */
    rawListenerCount--;
    if (listenerIndex < rawListenerCount) 
      memcpy(&rawListenerList[listenerIndex], &rawListenerList[listenerIndex + 1],
	     sizeof(rawListener) * (rawListenerCount - listenerIndex));
    
    /* And we are done */
    return TRUE;
  }

  /* Didn't find it! */
  return FALSE;
}

/* Dispatch raw messages */
Bool xPL_dispatchRawEvent(String theData, int dataLen) {
  Bool messageDispatched = FALSE;
  rawListenerPtr theRawListener;
  int listenerIndex;

  for (listenerIndex = rawListenerCount - 1; listenerIndex >= 0; listenerIndex--) {
    theRawListener = &rawListenerList[listenerIndex];
    
    /* Dispatch the message */
    theRawListener->theListener(theData, dataLen, theRawListener->userValue);
    messageDispatched = TRUE;
  }

  /* And we are done */
  return messageDispatched;
}

/* Add a message listener */
void xPL_addMessageListener(xPL_messageListener theHandler, xPL_ObjectPtr userValue) {
  messageListenerPtr theListener;

  /* See if there is a slot to install and allocate more if not */
  if (messageListenerCount == messageAllocListenerCount) {
    messageAllocListenerCount += GROW_LIST_BY;
    messageListenerList = realloc(messageListenerList, sizeof(messageListener) * messageAllocListenerCount);
  }
  theListener = &messageListenerList[messageListenerCount++];
  bzero(theListener, sizeof(messageListener));

  /* Install values */
  theListener->theListener = theHandler;
  theListener->userValue = userValue;
}

/* Remove a message listener */
Bool xPL_removeMessageListener(xPL_messageListener theHandler) {
  int listenerIndex;
  messageListenerPtr theListener;

  for (listenerIndex = 0; listenerIndex < messageListenerCount; listenerIndex++) {
    theListener = &messageListenerList[listenerIndex];
    if (theListener->theListener != theHandler) continue;
    
    /* Remove it from the listener list */
    messageListenerCount--;
    if (listenerIndex < messageListenerCount) 
      memcpy(&messageListenerList[listenerIndex], &messageListenerList[listenerIndex + 1],
	     sizeof(messageListener) * (messageListenerCount - listenerIndex));
    
    /* And we are done */
    return TRUE;
  }

  /* Didn't find it! */
  return FALSE;
}

/* Dispatch messages */
Bool xPL_dispatchMessageEvent(xPL_MessagePtr theMessage) {
  Bool messageDispatched = FALSE;
  messageListenerPtr theMessageListener;
  int listenerIndex;

  for (listenerIndex = messageListenerCount - 1; listenerIndex >= 0; listenerIndex--) {
    theMessageListener = &messageListenerList[listenerIndex];
    
    /* Dispatch the message */
    theMessageListener->theListener(theMessage, theMessageListener->userValue);
    messageDispatched = TRUE;
  }

  /* And we are done */
  return messageDispatched;
}

/* Add a service listener */
void xPL_addServiceListener(xPL_ServicePtr theService, xPL_ServiceListener theListener, xPL_MessageType messageType, String schemaClass, String schemaType, xPL_ObjectPtr userValue) {
  xPL_ServiceListenerDefPtr theServiceListener;

  /* See if there is a slot to install and allocate more if not */
  if (theService->listenerCount == theService->listenerAllocCount) {
    theService->listenerAllocCount += GROW_LIST_BY;
    theService->serviceListenerList = realloc(theService->serviceListenerList, sizeof(xPL_ServiceListenerDef) * theService->listenerAllocCount);
  }

  theServiceListener = &(theService->serviceListenerList[theService->listenerCount++]);
  bzero(theServiceListener, sizeof(xPL_ServiceListenerDef));

  /* Install values */
  theServiceListener->serviceListener = theListener;
  theServiceListener->matchMessageType = messageType;

  /* Install optional schema related matches */
  if ((schemaClass != NULL) && (strlen(schemaClass) > 0)) {
    theServiceListener->matchSchemaClass = xPL_StrDup(schemaClass);
  }
  if ((schemaType != NULL) && (strlen(schemaType) > 0)) {
    theServiceListener->matchSchemaType = xPL_StrDup(schemaType);
  }

  theServiceListener->userValue = userValue;
}

/* Remove a service listener */
Bool xPL_removeServiceListener(xPL_ServicePtr theService, xPL_ServiceListener theListener) {
  int listenerIndex;
  xPL_ServiceListenerDefPtr theServiceListener;

  for (listenerIndex = 0; listenerIndex < theService->listenerCount; listenerIndex++) {
    theServiceListener = (xPL_ServiceListenerDefPtr) &(theService->serviceListenerList[listenerIndex]);
    if (theServiceListener->serviceListener != theListener) continue;
    
    /* Free resources */
    STR_FREE(theServiceListener->matchSchemaClass);
    STR_FREE(theServiceListener->matchSchemaType);

    /* Remove it from the listener list */
    theService->listenerCount--;
    if (listenerIndex < theService->listenerCount) 
      memcpy(&(theService->serviceListenerList[listenerIndex]), &(theService->serviceListenerList[listenerIndex + 1]),
	     sizeof(xPL_ServiceListenerDef) * (theService->listenerCount - listenerIndex));
    
    /* And we are done */
    return TRUE;
  }

  /* Didn't find it! */
  return FALSE;
}

/* Dispatch service messages to appropriate listeners*/
Bool xPL_dispatchServiceEvent(xPL_ServicePtr theService, xPL_MessagePtr theMessage) {
  Bool messageDispatched = FALSE;
  xPL_ServiceListenerDefPtr theListener;
  int listenerIndex;

  for (listenerIndex = theService->listenerCount - 1; listenerIndex >= 0; listenerIndex--) {
    theListener = &(theService->serviceListenerList[listenerIndex]);

    /* Skip on incorrect message types */
    if (theListener->matchMessageType != xPL_MESSAGE_ANY) {
      if (theListener->matchMessageType != theMessage->messageType) continue;
    }
    
    /* Do a Schema class check */
    if (theListener->matchSchemaClass != NULL) {
      if (xPL_strcmpIgnoreCase(theListener->matchSchemaClass, theMessage->schemaClass) != 0) continue;
    }

    /* Do a Schema type check */
    if (theListener->matchSchemaType != NULL) {
      if (xPL_strcmpIgnoreCase(theListener->matchSchemaType, theMessage->schemaType) != 0) continue;
    }

    /* Dispatch the message */
    ((xPL_ServiceListener) theListener->serviceListener)(theService, theMessage, theListener->userValue);
    messageDispatched = TRUE;
  }

  /* And we are done */
  return messageDispatched;
}


/**** Configuration Listeners *****/

/* Add a service config change listener */
void xPL_addServiceConfigChangedListener(xPL_ServicePtr theService, xPL_ServiceConfigChangedListener theListener, xPL_ObjectPtr userValue) {
  xPL_ServiceChangedListenerDefPtr theChangeListener;

  /* See if there is a slot to install and allocate more if not */
  if (theService->configChangedCount == theService->configChangedAllocCount) {
    theService->configChangedAllocCount += GROW_LIST_BY;
    theService->changedListenerList = realloc(theService->changedListenerList, sizeof(xPL_ServiceChangedListenerDef) * theService->configChangedAllocCount);
  }

  theChangeListener = &(theService->changedListenerList[theService->configChangedCount++]);
  bzero(theChangeListener, sizeof(xPL_ServiceChangedListenerDef));

  /* Install values */
  theChangeListener->changeListener = theListener;
  theChangeListener->userValue = userValue;
}

/* Remove a config change listener */
Bool xPL_removeServiceConfigChangedListener(xPL_ServicePtr theService, xPL_ServiceConfigChangedListener theListener) {
  int listenerIndex;
  xPL_ServiceChangedListenerDefPtr theChangeListener;

  for (listenerIndex = 0; listenerIndex < theService->configChangedCount; listenerIndex++) {
    theChangeListener = (xPL_ServiceChangedListenerDefPtr) &(theService->changedListenerList[listenerIndex]);
    if (theChangeListener->changeListener != theListener) continue;
    
    /* Remove it from the listener list */
    theService->configChangedCount--;
    if (listenerIndex < theService->configChangedCount) 
      memcpy(&(theService->changedListenerList[listenerIndex]), &(theService->changedListenerList[listenerIndex + 1]),
	     sizeof(xPL_ServiceChangedListenerDef) * (theService->configChangedCount - listenerIndex));
    
    /* And we are done */
    return TRUE;
  }

  /* Didn't find it! */
  return FALSE;
}

/* Dispatch service messages to appropriate listeners*/
Bool xPL_dispatchServiceConfigChangedEvent(xPL_ServicePtr theService) {
  Bool messageDispatched = FALSE;
  xPL_ServiceChangedListenerDefPtr theListener;
  int listenerIndex;

  for (listenerIndex = theService->configChangedCount - 1; listenerIndex >= 0; listenerIndex--) {
    theListener = &(theService->changedListenerList[listenerIndex]);

    /* Dispatch the message */
    ((xPL_ServiceConfigChangedListener) theListener->changeListener)(theService, theListener->userValue);
    messageDispatched = TRUE;
  }

  /* And we are done */
  return messageDispatched;
}

