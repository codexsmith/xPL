/* xPL_Logger.c - Simple program to monitor for any message and any device changes and print them */
/* Copyright (c) 2004, Gerald R. Duprey Jr. */

#include <stdio.h>
#include <signal.h>
#include "../xPL.h"

#define LOGGER_VERSION "1.1"

#define LOG_FILE_CFG_NAME "logFilename"
#define LOG_APPEND_CFG_NAME "append2Log"

static FILE * logFile = NULL;
static String logFileName = "";
static Bool appendToLog = FALSE;

static xPL_ServicePtr loggerService = NULL;

void shutdownHandler(int onSignal) {
  xPL_setServiceEnabled(loggerService, FALSE);
  xPL_releaseService(loggerService);
  xPL_shutdown();
  exit(0);
}


/** It's best to put the logic for reading the service configuration */
/** and parsing it into your code in a seperate function so it can   */
/** be used by your configChangedHandler and your startup code that  */
/** will want to parse the same data after a config file is loaded   */
static void parseConfig(xPL_ServicePtr theService) {
  String configValue;
  FILE * newLogFile = NULL;

  /* Get append status */
  if ((configValue = xPL_getServiceConfigValue(theService, LOG_APPEND_CFG_NAME)) != NULL) {
    appendToLog = xPL_strcmpIgnoreCase(configValue, "true") == 0;
  }
 
  /* Get log file name and see if it's changed */
  if ((configValue = xPL_getServiceConfigValue(theService, LOG_FILE_CFG_NAME)) != NULL) {
    /* Log file changed and not blank -- try to open the new one */
    if ((strlen(configValue) != 0) && (strcmp(logFileName, configValue) != 0)) {
      /* Attempt to open new log file */
      if (xPL_strcmpIgnoreCase(configValue, "stderr") == 0) {
	newLogFile = stderr;
      } else if (xPL_strcmpIgnoreCase(configValue, "stdout") == 0) {
	newLogFile = stdout;
      } else {
	newLogFile = fopen(configValue, (appendToLog ? "a" : "w"));
      }

      /* If there is no new log file, it's bad -- ignore this and move on */
      if (newLogFile == NULL) return;
	  
      /* Close out old log file (unless it was the console */
      if ((logFile != NULL) && (logFile != stderr) && (logFile != stdout)) {
	fflush(logFile);
	fclose(logFile);
      }
      
      /** Install new file name and handle */
      logFileName = configValue;
      logFile = newLogFile;
    }
  }

  /** Other configurables */
}

/** Handle a change to the logger service configuration */
static void configChangedHandler(xPL_ServicePtr theService, xPL_ObjectPtr userData) {
  /* Read config items for service and install */
  parseConfig(theService);
}

/* Write a date/time stamp */
void printTimestamp() {
  char dateTimeBuffer[41];
  time_t rightNow;

  time(&rightNow);
  strftime(dateTimeBuffer, 40, "%y/%m/%d %H:%M:%S ", localtime(&rightNow));
  fprintf(logFile, dateTimeBuffer);
}

/* Print info on incoming messages */
void printXPLMessage(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue) {
  printTimestamp();
  fprintf(logFile, "[xPL_MSG] TYPE=");
  switch(xPL_getMessageType(theMessage)) {
  case xPL_MESSAGE_COMMAND:
    fprintf(logFile, "xpl-cmnd");
    break;
  case xPL_MESSAGE_STATUS:
    fprintf(logFile, "xpl-stat");
    break;
  case xPL_MESSAGE_TRIGGER:
    fprintf(logFile, "xpl-trig");
    break;
  default:
    fprintf(logFile, "!UNKNOWN!");
    break;
  }


  /* Print hop count, if interesting */
  if (xPL_getHopCount(theMessage) != 1) fprintf(logFile, ", HOPS=%d", xPL_getHopCount(theMessage));
  
  /* Source Info */
  fprintf(logFile, ", SOURCE=%s-%s.%s, TARGET=", 
	  xPL_getSourceVendor(theMessage),
	  xPL_getSourceDeviceID(theMessage),
	  xPL_getSourceInstanceID(theMessage));

  /* Handle various target types */
  if (xPL_isBroadcastMessage(theMessage)) {
    fprintf(logFile, "*");
  } else {
    if (xPL_isGroupMessage(theMessage)) {
      fprintf(logFile, "XPL-GROUP.%s", xPL_getTargetGroup(theMessage));
    } else {
      fprintf(logFile, "%s-%s.%s", 
	      xPL_getTargetVendor(theMessage),
	      xPL_getTargetDeviceID(theMessage),
	      xPL_getTargetInstanceID(theMessage));
    }
  }

  /* Echo Schema Info */
  fprintf(logFile, ", CLASS=%s, TYPE=%s", xPL_getSchemaClass(theMessage), xPL_getSchemaType(theMessage));
  fprintf(logFile, "\n");
}

int main(int argc, String argv[]) {
  /* Parse the command line */
  if (!xPL_parseCommonArgs(&argc, argv, FALSE)) exit(1);

  /* Start xPLLib */
  if (!xPL_initialize(xPL_getParsedConnectionType())) {
    fprintf(stderr, "Unable to start xPLLib\n");
    exit(1);
  }

  /* And a listener for all xPL messages */
  xPL_addMessageListener(printXPLMessage, NULL);

  /* Create a service so the hubs know to send things to us        */
  /* While we are not relaly using he service, xPL hubs will not   */
  /* forward messages to us until they have seen a xPL-looking     */
  /* device on the end of a hub connection, so this just gets us a */
  /* place at the table, so to speak                               */
  loggerService = xPL_createConfigurableService("cdp1802", "logger", "logger.xpl");
  xPL_setServiceVersion(loggerService, LOGGER_VERSION);

  if (!xPL_isServiceConfigured(loggerService)) {
    /* Define a configurable item and give it a default */
    xPL_addServiceConfigurable(loggerService, LOG_FILE_CFG_NAME, xPL_CONFIG_RECONF, 1);
    xPL_setServiceConfigValue(loggerService, LOG_FILE_CFG_NAME, "stderr");

    xPL_addServiceConfigurable(loggerService, LOG_APPEND_CFG_NAME, xPL_CONFIG_RECONF, 1);
    xPL_setServiceConfigValue(loggerService, LOG_APPEND_CFG_NAME, "false");
  }

  /* Parse the service configurables into a form this program */
  /* can use (whether we read a config or not)                */
  parseConfig(loggerService);

  /* Add a service change listener we'll use to pick up changes */
  xPL_addServiceConfigChangedListener(loggerService, configChangedHandler, NULL);

  /** Enable the service */
  xPL_setServiceEnabled(loggerService, TRUE);

  /* Install signal traps for proper shutdown */
  signal(SIGTERM, shutdownHandler);
  signal(SIGINT, shutdownHandler);

  /* Hand control over to xPLLib */
  xPL_processMessages(-1);
  exit(0);
}
