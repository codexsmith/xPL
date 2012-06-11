/* xPL_priv.h - xPLLib internal include */
/* Copyright 2004 (c), Gerald R Duprey Jr */

#define SAFE_FREE(x) if (x != NULL) { free(x); x = NULL; }
#define STR_FREE(x) if (x != NULL) { xPL_FreeStr(x); x = NULL; }

extern int xPLFD;   /* FD We are connecting on */
extern pid_t xPLPID;
extern Bool xPL_DebugMode;

/* xPL-utils.c */
extern xPL_NameValueListPtr xPL_newNamedValueList();
extern void xPL_freeNamedValueList(xPL_NameValueListPtr);
extern xPL_NameValuePairPtr xPL_newNamedValuePair(xPL_NameValueListPtr, String);
extern void xPL_freeNamedValuePair(xPL_NameValuePairPtr);

/* xPL-service.c */
extern Bool xPL_sendHeartbeat(xPL_ServicePtr);
extern Bool xPL_sendGoodbyeHeartbeat(xPL_ServicePtr);
extern void xPL_sendTimelyHeartbeats();
extern void xPL_handleServiceMessage(xPL_MessagePtr, xPL_ObjectPtr);
extern void xPL_releaseServiceConfigurables(xPL_ServicePtr theService);
extern void xPL_disableAllServices();


/* xPL-message.c */
extern void xPL_receiveMessage(int, int, int);

/* xPL-listeners.c */
extern Bool xPL_dispatchRawEvent(String, int);
extern Bool xPL_dispatchMessageEvent(xPL_MessagePtr);
extern Bool xPL_dispatchServiceEvent(xPL_ServicePtr, xPL_MessagePtr);
extern Bool xPL_dispatchServiceConfigChangedEvent(xPL_ServicePtr);

/* xPL-io.c */
extern Bool xPL_sendRawMessage(String, int);
extern String xPL_getFairlyUniqueIdent();

/* xPL-store.c */
extern void xPL_FreeNVPair(xPL_NameValuePairPtr);
extern xPL_NameValuePairPtr xPL_AllocNVPair();

extern void xPL_FreeNVList(xPL_NameValueListPtr);
extern xPL_NameValueListPtr xPL_AllocNVList();

extern void xPL_FreeMessage(xPL_MessagePtr);
extern xPL_MessagePtr xPL_AllocMessage();

extern void xPL_FreeService(xPL_ServicePtr);
extern xPL_ServicePtr xPL_AllocService();

extern void xPL_FreeStr(String);
extern String xPL_StrDup(String);
extern String xPL_StrNDup(String, int);
extern String xPL_StrAlloc(int);

/* xPL-config.c */
extern void xPL_releaseServiceConfig(xPL_ServicePtr);
