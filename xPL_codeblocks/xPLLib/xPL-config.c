/* xPL-config.c -- Configuration support for xPL services */
/* Copyright (c) 2004, Gerald R Duprey Jr.  */

#include <stdio.h>

#include "xPL.h"
#include "xPL_priv.h"

#define GROW_CONFIG_LIST_BY 4

/* Flag set when reading in a configuration to bypass some normal */
/* safety checks (like no changes to the config after the service */
/* is enabled). */
static Bool configurationBeingInstalled = FALSE;

/* Forward declaration */
static Bool xPL_saveServiceConfig(xPL_ServicePtr theService);


/* Send list of configurables */
static void sendServiceConfigList(xPL_ServicePtr theService) {
  int configIndex;
  int allocSize;
  String configType, configName;
  xPL_MessagePtr theMessage = xPL_createBroadcastMessage(theService, xPL_MESSAGE_STATUS);
  xPL_ServiceConfigurablePtr configurable;

  /* Set schema type */
  xPL_setSchema(theMessage, "config", "list");
  
  /* Add in standard configurables */
  xPL_addMessageNamedValue(theMessage, "reconf", "newconf");
  xPL_addMessageNamedValue(theMessage, "reconf", "interval");
  xPL_addMessageNamedValue(theMessage, "option", "group[255]");
  xPL_addMessageNamedValue(theMessage, "option", "filter[255]");
 
  /* Add in service configurable */
  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    configurable = &(theService->configList[configIndex]);
    
    /* Figure out what type of configurable this is */
    switch(configurable->itemType) {
    case xPL_CONFIG_OPTIONAL:
      configType = "option";
      break;
    case xPL_CONFIG_MANDATORY:
      configType = "config";
      break;
    case xPL_CONFIG_RECONF:
      configType = "reconf";
      break;

    default:
      /* If we don't know what it is, skip it */
      continue;
    }
    
    /* Set the configurable name */
    configName = configurable->itemName;
    if (configurable->maxValueCount > 1) {
      /* Figure out how large the string needs to be */
      allocSize = strlen(configName);
      allocSize += 3;
      if (configurable->maxValueCount > 9) allocSize++;
      if (configurable->maxValueCount > 99) allocSize++;
      if (configurable->maxValueCount > 999) allocSize++;
      if (configurable->maxValueCount > 9999) allocSize++;
    
      /* Allocate a string and format it */
      configName = xPL_StrAlloc(allocSize);
      sprintf(configName, "%s[%d]", configurable->itemName, configurable->maxValueCount);
      
      /* Free temp string, if any */
      if (configName != configurable->itemName) STR_FREE(configName);
    }

    /* Add the item in */
    xPL_addMessageNamedValue(theMessage, configType, configName);
  }

  /* Send out the message */
  xPL_sendMessage(theMessage);

  /* And release it */
  xPL_releaseMessage(theMessage);
}

/* Convert passed filter into a string.  String is dynamic */
/* and must be freed when no longer needed                 */
static String formatFilter(xPL_ServiceFilterPtr theFilter) {
  int allocSize = 5;
  String formattedFilter;

  /* See how big this needs to be */
  allocSize += (theFilter->matchOnMessageType == xPL_MESSAGE_ANY ? 1 : 8);
  allocSize += (theFilter->matchOnVendor == NULL ? 1 : strlen(theFilter->matchOnVendor));
  allocSize += (theFilter->matchOnDeviceID == NULL ? 1 : strlen(theFilter->matchOnDeviceID));
  allocSize += (theFilter->matchOnInstanceID == NULL ? 1 : strlen(theFilter->matchOnInstanceID));
  allocSize += (theFilter->matchOnSchemaClass == NULL ? 1 : strlen(theFilter->matchOnSchemaClass));
  allocSize += (theFilter->matchOnSchemaType == NULL ? 1 : strlen(theFilter->matchOnSchemaType));

  /* Allocate it */
  formattedFilter = xPL_StrAlloc(allocSize);

  /* Format message type */
  switch(theFilter->matchOnMessageType) {
  case xPL_MESSAGE_ANY:
    strcat(formattedFilter, "*");
    break;
  case xPL_MESSAGE_COMMAND:
    strcat(formattedFilter, "xpl-cmnd");
    break;
  case xPL_MESSAGE_STATUS:
    strcat(formattedFilter, "xpl-stat");
    break;
  case xPL_MESSAGE_TRIGGER:
    strcat(formattedFilter, "xpl-trig");
    break;
  }

  /* Format other filters */
  strcat(formattedFilter, ".");
  strcat(formattedFilter, (theFilter->matchOnVendor == NULL ? "*" : theFilter->matchOnVendor));
  strcat(formattedFilter, ".");
  strcat(formattedFilter, (theFilter->matchOnDeviceID == NULL ? "*" : theFilter->matchOnDeviceID));
  strcat(formattedFilter, ".");
  strcat(formattedFilter, (theFilter->matchOnInstanceID == NULL ? "*" : theFilter->matchOnInstanceID));
  strcat(formattedFilter, ".");
  strcat(formattedFilter, (theFilter->matchOnSchemaClass == NULL ? "*" : theFilter->matchOnSchemaClass));
  strcat(formattedFilter, ".");
  strcat(formattedFilter, (theFilter->matchOnSchemaType == NULL ? "*" : theFilter->matchOnSchemaType));

  /* Return it */
  return formattedFilter;
}

/* Parse a string into a filter */
static Bool parseFilter(String theText, xPL_ServiceFilterPtr theFilter) {
  String msgType, vendor, device, instance;
  String schemaClass, schemaType;

  msgType = theText;

  if ((vendor = strstr(msgType, ".")) == NULL) return FALSE;
  vendor++;

  if ((device = strstr(vendor, ".")) == NULL) return FALSE;
  device++;

  if ((instance = strstr(device, ".")) == NULL) return FALSE;
  instance++;

  if ((schemaClass = strstr(instance, ".")) == NULL) return FALSE;
  schemaClass++;

  if ((schemaType = strstr(schemaClass, ".")) == NULL) return FALSE;
  schemaType++;

  /* Install message type */
  if (strncmp(msgType, "*", 1) == 0) {
    theFilter->matchOnMessageType = xPL_MESSAGE_ANY;
  } else if (xPL_strncmpIgnoreCase(msgType, "xpl-cmnd", 8) == 0) {
    theFilter->matchOnMessageType = xPL_MESSAGE_COMMAND;
  } else if (xPL_strncmpIgnoreCase(msgType, "xpl-stat", 8) == 0) {
    theFilter->matchOnMessageType = xPL_MESSAGE_STATUS;
  } else if (xPL_strncmpIgnoreCase(msgType, "xpl-trig", 8) == 0) {
    theFilter->matchOnMessageType = xPL_MESSAGE_TRIGGER;
  } else {
    /* Bad filter */
    return FALSE;
  }

  /* Install device filters */
  theFilter->matchOnVendor = (strncmp(vendor, "*", 1) == 0 ? NULL : xPL_StrNDup(vendor, (device - vendor) - 1));
  theFilter->matchOnDeviceID = (strncmp(device, "*", 1) == 0 ? NULL : xPL_StrNDup(device, (instance - device) - 1));
  theFilter->matchOnInstanceID = (strncmp(instance, "*", 1) == 0 ? NULL : xPL_StrNDup(instance, (schemaClass - instance) - 1));

  /* Install schema filters */
  theFilter->matchOnSchemaClass = (strncmp(schemaClass, "*", 1) == 0 ? NULL : xPL_StrNDup(schemaClass, (schemaType - schemaClass) - 1));
  theFilter->matchOnSchemaType = (strncmp(schemaType, "*", 1) == 0 ? NULL : xPL_StrDup(schemaType));

  /* And we are done */
  return TRUE;
}

/* Send list of configurables */
static void sendServiceCurrentConfig(xPL_ServicePtr theService) {
  int groupIndex, filterIndex;
  int configIndex;
  int valueIndex, valueCount;
  char intervalBuff[8];
  String filterText;
  xPL_MessagePtr theMessage = xPL_createBroadcastMessage(theService, xPL_MESSAGE_STATUS);
  xPL_ServiceConfigurablePtr configurable;
  xPL_ServiceFilterPtr theFilter;

  /* Set schema type */
  xPL_setSchema(theMessage, "config", "current");
  
  /* Set instance ID */
  xPL_addMessageNamedValue(theMessage, "newconf", xPL_getServiceInstanceID(theService));
  
  /* Include interval */
  sprintf(intervalBuff, "%d", xPL_getHeartbeatInterval(theService) / 60);
  xPL_addMessageNamedValue(theMessage, "interval", intervalBuff);

  /* Include groups */
  if (theService->groupCount == 0) 
    xPL_addMessageNamedValue(theMessage, "group", NULL);
  else {
    for (groupIndex = 0; groupIndex < theService->groupCount; groupIndex++) {
      xPL_addMessageNamedValue(theMessage, "group", theService->groupList[groupIndex]);
    }
  }

  /* Include filters */
  if (theService->filterCount == 0)
    xPL_addMessageNamedValue(theMessage, "filter", NULL);
  else {
    for (filterIndex = 0; filterIndex < theService->filterCount; filterIndex++) {
      theFilter = &(theService->messageFilterList[filterIndex]);
      filterText = formatFilter(theFilter);
      xPL_addMessageNamedValue(theMessage, "filter", filterText);
      STR_FREE(filterText);
    }
  }
 
  /* Add in service configurables */
  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    configurable = &(theService->configList[configIndex]);
    valueCount = 0;

    /* Write each value out */
    for (valueIndex = 0; valueIndex < configurable->valueCount; valueIndex++) {
      /* Skip NULL values */
      if (configurable->valueList[valueIndex] == NULL) continue;

      /* Write value out */
      xPL_addMessageNamedValue(theMessage, configurable->itemName, configurable->valueList[valueIndex]);
      valueCount++;
    }

    /* If no values were written out, put out a place holder for this named configurable */
    if (valueCount == 0) xPL_addMessageNamedValue(theMessage, configurable->itemName, NULL);
  }

  /* Send out the message */
  xPL_sendMessage(theMessage);

  /* And release it */
  xPL_releaseMessage(theMessage);
}

/* Parse a configurable definition from the passed string */
static Bool parseConfigDefinition(xPL_ServicePtr theService, String parseValue, int configType) {
  int valueCount, nameLength = strlen(parseValue);
  String startChar = NULL, endChar;
  char nameBuffer[128];

  /* See how many values are allowed */
  valueCount = 1;
  if ((startChar = strstr(parseValue, "[")) != NULL) {
    nameLength = (startChar - parseValue);
    startChar++;

    valueCount = strtol(startChar, &endChar, 10);
    if (*endChar != ']') {
      xPL_Error("CONFIG:: Bad defintion for config=%s[%s -- item ignored", parseValue, startChar);
      return FALSE;
    }

    /* Shouldn't happen, but be safe */
    if (valueCount < 1) valueCount = 1;
  }

  /* For for any of the "built in" items and skip them */
  if (xPL_strncmpIgnoreCase(parseValue, "newconf", nameLength) == 0) return TRUE;
  if (xPL_strncmpIgnoreCase(parseValue, "interval", nameLength) == 0) return TRUE;
  if (xPL_strncmpIgnoreCase(parseValue, "filter", nameLength) == 0) return TRUE;
  if (xPL_strncmpIgnoreCase(parseValue, "group", nameLength) == 0) return TRUE;

  /* Try to add a new configurable */
  strncpy(nameBuffer, parseValue, nameLength);
  nameBuffer[nameLength] = '\0';
  if (!xPL_addServiceConfigurable(theService, nameBuffer, configType, valueCount)) {
    xPL_Error("CONFIG:: Unable to define configurable %s -- item ignored", parseValue);
    return FALSE;
  }

  /* Success! */
  xPL_Debug("CONFIG:: Added new configurable item %s", parseValue);
  return TRUE;
}

/* Handle new configuration data */
static void installNewConfig(xPL_ServicePtr theService, xPL_NameValueListPtr namedValues, Bool allowDefinitions) {
  Bool foundFilter = FALSE;
  Bool foundGroup = FALSE;

  Bool serviceWasEnabled = theService->serviceEnabled;
  Bool restartService = FALSE;
  String newInstanceID = NULL;
  int newInterval = -1;

  xPL_NameValuePairPtr theElement;

  int elementCount = xPL_getNamedValueCount(namedValues);
  int elementIndex;

  /* Flag that allows us to bypass the internal "no-change" checks */
  configurationBeingInstalled = TRUE;

  /* Clear current config values */
  xPL_clearAllServiceConfigValues(theService);

  /* Iterate over each element */
  for (elementIndex = 0; elementIndex < elementCount; elementIndex++) {
    if ((theElement = xPL_getNamedValuePairAt(namedValues, elementIndex)) == NULL) continue;

    /* Check for definitional items */

    /* Mandatory items */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "config") == 0) {
      if (!allowDefinitions) continue;
      
      parseConfigDefinition(theService, theElement->itemValue, xPL_CONFIG_MANDATORY);
      continue;
    }

    /* Mandatory, reconfigurable items */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "reconf") == 0) {
      if (!allowDefinitions) continue;
      
      parseConfigDefinition(theService, theElement->itemValue, xPL_CONFIG_RECONF);
      continue;
    }

    /* Optional Items */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "option") == 0) {
      if (!allowDefinitions) continue;
      
      parseConfigDefinition(theService, theElement->itemValue, xPL_CONFIG_OPTIONAL);
      continue;
    }

    /* Check for instance Change */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "newconf") == 0) {
      /* See if any change */
      if (strcmp(theElement->itemValue, theService->serviceInstanceID) == 0) continue;

      /* Get the new ID */
      newInstanceID = theElement->itemValue;
      xPL_Debug("CONFIG:: Got new instanceID %s for service -- will restart service\n", newInstanceID);
      restartService = TRUE;
      continue;
    }

    /* Check for interval change */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "interval") == 0) {
      /* See if any change */
      if (!xPL_strToInt(theElement->itemValue, &newInterval)) continue;
      newInterval *= 60;   /* Convert to seconds */

      /* If no change, then skip it */
      if (newInterval == theService->heartbeatInterval) {
	newInterval = -1;
	continue;
      }

      xPL_Debug("CONFIG:: Got new interval of %d for service -- will restart service\n", newInterval);
      restartService = TRUE;
      continue;
    }

    /* Check for filters */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "filter") == 0) {
      /* See if this is the first filter we have */
      if (!foundFilter) xPL_clearServiceFilters(theService);
      foundFilter = TRUE;

      /* Skip if not actual filter */
      if (strlen(theElement->itemValue) == 0) continue;
    
      /* Add to the filter list */
      if (theService->filterCount == theService->filterAllocCount) {
	theService->filterAllocCount += 4;
	theService->messageFilterList = realloc(theService->messageFilterList, 
						sizeof(xPL_ServiceFilter) * theService->filterAllocCount);
      }

      /* Parse the filter and validate it */
      if (!parseFilter(theElement->itemValue, &(theService->messageFilterList[theService->filterCount]))) continue;
      theService->filterCount++;
      xPL_Debug("CONFIG:: Added new filter %s to service\n", theElement->itemValue);

      continue;
    }

    /* Check for groups */
    if (xPL_strcmpIgnoreCase(theElement->itemName, "group") == 0) {
      /* See if this is the first group we have */
      if (!foundGroup) xPL_clearServiceGroups(theService);
      foundGroup = TRUE;
    
      /* Skip if no actual group */
      if (strlen(theElement->itemValue) == 0) continue;

      /* Insure we have space for this */
      if (theService->groupCount == theService->groupAllocCount) {
	theService->groupAllocCount += 4;
	theService->groupList = (String *) realloc(theService->groupList, sizeof(String) * theService->groupAllocCount);
      }
      
      /* Add the group */
      theService->groupList[theService->groupCount++] = xPL_StrDup(theElement->itemValue);
      xPL_Debug("CONFIG:: Added new group %s to service\n", theElement->itemValue);
      continue;
    }


    /* Anything else had better be a configurable */
    xPL_Debug("CONFIG:: About to update configurable %s with value %s\n", theElement->itemName, theElement->itemValue);
    if (!xPL_addServiceConfigValue(theService, theElement->itemName, theElement->itemValue)) {
      xPL_Debug("CONFIG::    UNABLE to add/set that configuration value!\n");
    }
  }

  /* Clear config install flag */
  configurationBeingInstalled = FALSE;

  /* See if we need to restart the service */
  if (serviceWasEnabled && restartService) {
    /* Disable service as is */
    xPL_Debug("CONFIG:: Stopping service to restart later for new instance/interval change\n");
    xPL_setServiceEnabled(theService, FALSE);
  }
   
  /* Install new values */
  if (newInstanceID != NULL) {
    STR_FREE(theService->serviceInstanceID);
    theService->serviceInstanceID = xPL_StrDup(newInstanceID);
  }
  if (newInterval != -1) theService->heartbeatInterval = newInterval;

  /* Flag the service as configured */
  theService->serviceConfigured = TRUE;

  /* Preserve settings */
  if (!allowDefinitions) xPL_saveServiceConfig(theService);

  /* Restart service, if needed */
  if (serviceWasEnabled && restartService) xPL_setServiceEnabled(theService, TRUE);
  xPL_Debug("CONFIG:: Restarted service with new parameters");

  /* Fire config changed message */
  if (serviceWasEnabled && !allowDefinitions) xPL_dispatchServiceConfigChangedEvent(theService);
}


/* Handle configuration messages */
static void configHandler(xPL_ServicePtr theService, xPL_MessagePtr theMessage, xPL_ObjectPtr userValue) {
  String theSchemaType = xPL_getSchemaType(theMessage);
  String theCommand = xPL_getMessageNamedValue(theMessage, "command");

  /* See if this is a request for a list of configurable elements */
  if ((xPL_getMessageType(theMessage) == xPL_MESSAGE_COMMAND) && (xPL_strcmpIgnoreCase(theSchemaType, "list") == 0)
      && ((theCommand != NULL) && (xPL_strcmpIgnoreCase(theCommand, "request") == 0))) {
    sendServiceConfigList(theService);
    return;
  }

  /* See if this is a request for current configuration values */
  if ((xPL_getMessageType(theMessage) == xPL_MESSAGE_COMMAND) && (xPL_strcmpIgnoreCase(theSchemaType, "current") == 0)
      && ((theCommand != NULL) && (xPL_strcmpIgnoreCase(theCommand, "request") == 0))) {
    sendServiceCurrentConfig(theService);
    return;
  }
  
  /* See if this is a response with our configuration data in it */
  if ((xPL_getMessageType(theMessage) == xPL_MESSAGE_COMMAND) && (xPL_strcmpIgnoreCase(theSchemaType, "response") == 0)) {
    installNewConfig(theService, xPL_getMessageBody(theMessage), FALSE);
    return;
  }
}

/* Install a new configuration file */
static void xPL_setServiceConfigFile(xPL_ServicePtr theService, String localConfigFile) {
  /* Skip unless there is a real change or service is disabled */
  if (theService->serviceEnabled 
   || ((theService->configFileName != NULL) && (localConfigFile != NULL) 
    && (strcmp(theService->configFileName, localConfigFile) == 0))) return;

  /* Free any old name */
  STR_FREE(theService->configFileName);

  /* Install new name */
  if (localConfigFile != NULL) theService->configFileName = xPL_StrDup(localConfigFile);
}

/* Return the installed config file, if any */
String xPL_getServiceConfigFile(xPL_ServicePtr theService) {
  return theService->configFileName;
}

/* Clear out existing configuration data and attempt to load it from */
/* the currently installed config file.  If there is no installed    */
/* config file, nothing happens.  If there is a file specified but   */
/* it does not exist, any previous config data is lost, but no error */
/* is thrown (it may be this is the first use of this file).         */
static Bool xPL_loadServiceConfig(xPL_ServicePtr theService) {
  FILE *configFile;
  char lineBuffer[2048];
  String readVendor, readDevice;
  String delimPtr;
  xPL_NameValueListPtr namedValues;


  /* Skip unless we have a local config file */
  if (theService->serviceEnabled 
   || (theService->configFileName == NULL) 
   || theService->serviceConfigured) return FALSE;

  /* Attempt to open the configuration file */
  if ((configFile = fopen(theService->configFileName, "r")) == NULL) {
    xPL_Debug("CONFIG:: File %s does not exist -- no config loaded", theService->configFileName);
    return TRUE;
  }

  /* Get the header and insure it matches */
  if (fgets(lineBuffer, 2048, configFile) == NULL) {
    xPL_Error("CONFIG:: Error reading config file %s -- config not loaded", theService->configFileName);
    fclose(configFile);
    return FALSE;
  }

  /* Whack the new line */
  lineBuffer[strlen(lineBuffer) - 1] = '\0';
  if (strlen(lineBuffer) < 5) {
    xPL_Error("CONFIG:: Config file header too short -- config not loaded");
    fclose(configFile);
    return FALSE;
  }

  /* Parse the header */
  xPL_Debug("CONFIG:: Read header %s from config file, about to parse", lineBuffer);
  readVendor = &lineBuffer[1];
  if ((readDevice = strstr(readVendor, "-")) == NULL) {
    xPL_Error("CONFIG:: Unable to parse configuration file header (missing dash) for %s -- config not loaded", theService->configFileName);
    fclose(configFile);
    return FALSE;
  }

  *readDevice++ = '\0';
  if (readDevice[strlen(readDevice) - 1] != ']') {
    xPL_Error("CONFIG:: Unable to parse configuration file header (missing end bracket) for %s -- config not loaded", theService->configFileName);
    fclose(configFile);
    return FALSE;
  }
  readDevice[strlen(readDevice) - 1] = '\0';

  /* Insure it matches */
  if ((xPL_strcmpIgnoreCase(readVendor, theService->serviceVendor) != 0) 
      || (xPL_strcmpIgnoreCase(readDevice, theService->serviceDeviceID) != 0)) {
    xPL_Error("CONFIG:: Config file header does not match this device -- config not loaded");
    fclose(configFile);
    return FALSE;
  }

  /* Create a place for all the file contents to go */
  namedValues = xPL_AllocNVList();

  /* Read in each line and pick it apart into an NV pair */
  for(;;) {
    /* Read in a line (checking fo EOF) */
    if (fgets(lineBuffer, 2048, configFile) == NULL) break;

    /* Fix end of line */
    if (lineBuffer[strlen(lineBuffer) - 1] == '\n') lineBuffer[strlen(lineBuffer) - 1] = '\0';

    /* Find the delimiter */
    if ((delimPtr = strstr(lineBuffer, "=")) == NULL) continue;

    /* Bash the line into two strings */
    *delimPtr++ = '\0';

    /* Add the line */
    xPL_addNamedValue(namedValues, lineBuffer, delimPtr);
  }

  /* Close the file */
  fclose(configFile);

  /* Parse the data into the service */
  if (xPL_getNamedValueCount(namedValues) > 0) {
    installNewConfig(theService, namedValues, TRUE);
  }

  /* Release resources */
  xPL_FreeNVList(namedValues);

  /* And we are done */
  xPL_Debug("CONFIG:: Config file successfully read and installed");
  return TRUE;
}

/* Save out the current configuration */
static Bool xPL_saveServiceConfig(xPL_ServicePtr theService) {
  int configIndex, configCount = theService->configCount;
  int valueIndex;
  char lineBuff[100];
  String theValue;

  xPL_ServiceConfigurablePtr configItem;
  FILE *configFile;

  /* Skip unless we have a local config file */
  if (theService->configFileName == NULL) return FALSE;

  /* Attempt to create the configuration file */
  if ((configFile = fopen(theService->configFileName, "w")) == NULL) {
    xPL_Debug("CONFIG:: File %s could not be opened/created -- %s (%d)", theService->configFileName, strerror(errno), errno);
    return FALSE;
  }

  /* write simple header data */
  fprintf(configFile, "[%s-%s]\n", theService->serviceVendor, theService->serviceDeviceID);
  fprintf(configFile, "reconf=newconf\n");
  fprintf(configFile, "reconf=interval\n");
  fprintf(configFile, "option=group[255]\n");
  fprintf(configFile, "option=filter[255]\n");
  
  /* write configurable headers */
  for (configIndex = 0; configIndex < configCount; configIndex++) {
    configItem = &(theService->configList[configIndex]);

    /* Write out configurable type */
    switch(configItem->itemType) {
    case xPL_CONFIG_OPTIONAL:
      strcpy(lineBuff, "option=");
      break;

    case xPL_CONFIG_MANDATORY:
      strcpy(lineBuff, "config=");
      break;

    case xPL_CONFIG_RECONF:
      strcpy(lineBuff, "reconf=");
      break;

    default:
      continue;
    }

    /* Write item name */
    strcat(lineBuff, configItem->itemName);

    /* See if there are multiple values allowed */
    if (configItem->maxValueCount > 1) {
      strcat(lineBuff, "[");
      strcat(lineBuff, xPL_intToStr(configItem->maxValueCount));
      strcat(lineBuff, "]");
    }

    /* Write the item */
    strcat(lineBuff, "\n");
    fprintf(configFile, lineBuff);
  }

  /* Now write current values out */
  fprintf(configFile, "newconf=%s\n", theService->serviceInstanceID);
  fprintf(configFile, "interval=%d\n", theService->heartbeatInterval / 60);

  /* Write filters, if any */
  if (theService->filterCount == 0) {
    fprintf(configFile, "filter=\n");
  } else {
    for (valueIndex = 0; valueIndex < theService->filterCount; valueIndex++) {
      theValue = formatFilter(&(theService->messageFilterList[valueIndex]));
      fprintf(configFile, "filter=%s\n", theValue);
      STR_FREE(theValue);
    }
  }
	     
  /* Write groups, if any */
  if (theService->groupCount == 0) {
    fprintf(configFile, "group=\n");
  } else {
    for (valueIndex = 0; valueIndex < theService->groupCount; valueIndex++) {
      fprintf(configFile, "group=%s\n", theService->groupList[valueIndex]);
    }
  }


  /* Write configurable values */
  for (configIndex = 0; configIndex < configCount; configIndex++) {
    configItem = &(theService->configList[configIndex]);

    for (valueIndex = 0; valueIndex < configItem->valueCount; valueIndex++) {
      fprintf(configFile, "%s=%s\n", configItem->itemName, configItem->valueList[valueIndex]);
    }
  }

  /* And we are done */
  fflush(configFile);
  fclose(configFile);
  return TRUE;
}

/* Create a new service and prepare it for configuration.  Like other */
/* services, this will still require being enabled to start.  Before  */
/* it's started, you need to define and attach the configurable items */
/* for the service.   When the service is enabled, if there is a      */
/* non-null configFile, it's values are read.  The services instance  */
/* value will be created in a fairly unique method for services that  */
/* have not yet been configured                                       */
xPL_ServicePtr xPL_createConfigurableService(String vendorName, String deviceID, String localConfigFile) {
  xPL_ServicePtr theService = NULL;
  
  /* Create the service */
  theService = xPL_createService(vendorName, deviceID, NULL);
  theService->configurableService = TRUE;
  theService->serviceConfigured = FALSE;

  /* Load configuration, if any */
  if (localConfigFile != NULL)  xPL_setServiceConfigFile(theService, localConfigFile);

  /* Install a configuration listener for this service */
  xPL_addServiceListener(theService, configHandler, xPL_MESSAGE_ANY, "config", NULL, NULL);

  /* If there is a config file, attempt to load it */
  if (theService->configFileName != NULL) xPL_loadServiceConfig(theService);

  /* And we are done */
  return theService;
}
 
/* Return TRUE if this is a configured service */
Bool xPL_isConfigurableService(xPL_ServicePtr theService) {
  return theService->configurableService;
}

/* Return TRUE if this is a configured service and */
/* a configuration has been received               */
Bool xPL_isServiceConfigured(xPL_ServicePtr theService) {
  return theService->serviceConfigured;
}

/* Search for a configurable and return it (or NULL) */
xPL_ServiceConfigurablePtr xPL_findServiceConfigurable(xPL_ServicePtr theService, String itemName) {
  int configIndex;
  xPL_ServiceConfigurablePtr theItem;

  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    theItem = &(theService->configList[configIndex]);
    if (xPL_strcmpIgnoreCase(theItem->itemName, itemName) == 0) return theItem;
  }

  return NULL;
}
      
/* Add a new configurable.  If the item is added, TRUE is returned.  If the item */
/* already exists, FALSE is returned and it's not added or altered              */
Bool xPL_addServiceConfigurable(xPL_ServicePtr theService, String itemName, xPL_ConfigurableType itemType, int maxValues) {
  /* Try to find it */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem != NULL) return FALSE;

  /* No new items while the service is enabled */
  if (theService->serviceEnabled) return FALSE;

  /* See if there is an item available */
  if (theService->configCount == theService->configAllocCount) {
    theService->configAllocCount += GROW_CONFIG_LIST_BY;
    theService->configList = realloc(theService->configList, sizeof(xPL_ServiceConfigurable) * theService->configAllocCount);
  }
  theItem = &(theService->configList[theService->configCount++]);

  /* Install values */
  theItem->itemName = xPL_StrDup(itemName);
  theItem->itemType = itemType;
  theItem->maxValueCount = (maxValues < 1 ? 1 : maxValues);
  theItem->valueList = NULL;
  theItem->valueAllocCount = 0;
  theItem->valueCount = 0;
  xPL_Debug("CONFIG:: Added configurable [%s] (%s), TYPE=%d, MAX_VALUES=%d", theItem->itemName, itemName, theItem->itemType, theItem->maxValueCount);

  /* And we are done */
  return TRUE;
}

/* Releases all resources for a passed item (values, names, etc) */
static void releaseConfigurable(xPL_ServiceConfigurablePtr theItem) {
  int valueIndex;

  /* Release item values */
  for (valueIndex = 0; valueIndex < theItem->valueCount; valueIndex++) {
    STR_FREE(theItem->valueList[valueIndex]);
  }
  
  /* Release item definitions */
  theItem->valueCount = 0;
  theItem->valueAllocCount = 0;
  SAFE_FREE(theItem->valueList);
  STR_FREE(theItem->itemName);
}

/* Remove a configurable.  Return TRUE if item found and removed, FALSE if */
/* not found                                                              */
Bool xPL_removeServiceConfigurable(xPL_ServicePtr theService, String itemName) {
  int configIndex;
  xPL_ServiceConfigurablePtr theItem;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled) return FALSE;

  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    theItem = &(theService->configList[configIndex]);
    if (xPL_strcmpIgnoreCase(theItem->itemName, itemName) != 0) continue;

    /* Release the items resources */
    releaseConfigurable(theItem);

    /* Shift things around */
    theService->configCount--;
    if (configIndex < theService->configCount) {
      memcpy(&(theService->configList[configIndex]), &(theService->configList[configIndex + 1]),
	     sizeof(xPL_ServiceConfigurable) * (theService->configCount - configIndex));
    }

    return TRUE;
  }

  return FALSE;
}

/* Remove all configurables */
void xPL_removeAllServiceConfigurables(xPL_ServicePtr theService) {
  int configIndex;
  xPL_ServiceConfigurablePtr theItem;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled) return;

  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    theItem = &(theService->configList[configIndex]);

    /* Release the items resources */
    releaseConfigurable(theItem);
  }
  theService->configCount = 0;
}

/* Release all configuration related resources */
void xPL_releaseServiceConfigurables(xPL_ServicePtr theService) {
  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled) return;

  /* Release configurables */
  xPL_removeAllServiceConfigurables(theService);

  /* Release service level allocation */
  SAFE_FREE(theService->configList);
  theService->configCount = 0;
  theService->configAllocCount = 0;
  
  STR_FREE(theService->configFileName);
  
  theService->configChangedCount = 0;
  theService->configChangedAllocCount = 0;
  SAFE_FREE(theService->changedListenerList);
}
  

/* Clear values for a given configurable */
void xPL_clearServiceConfigValues(xPL_ServicePtr theService, String itemName) {
  int valueIndex;

  /* Get item and if not found, bail */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem == NULL) return;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled && !configurationBeingInstalled) return;

  for (valueIndex = 0; valueIndex < theItem->valueCount; valueIndex++) {
    STR_FREE(theItem->valueList[valueIndex]);
  }
  theItem->valueCount = 0;
}

/* Clear all configurable values out.  The configurable definitions */
/* remain in tact                                                 */
void xPL_clearAllServiceConfigValues(xPL_ServicePtr theService) {
  int configIndex, valueIndex;
  xPL_ServiceConfigurablePtr theItem;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled && !configurationBeingInstalled) return;

  for (configIndex = 0; configIndex < theService->configCount; configIndex++) {
    theItem = &(theService->configList[configIndex]);

    for (valueIndex = 0; valueIndex < theItem->valueCount; valueIndex++) {
      STR_FREE(theItem->valueList[valueIndex]);
    }
    theItem->valueCount = 0;
  }
}


/* Add a service item value.  If there are already values */
/* this is added to it, up to the limit defined for the   */
/* configurable.  If the item is "full", then the value is */
/* discarded                                              */
Bool xPL_addServiceConfigValue(xPL_ServicePtr theService, String itemName, String itemValue) {
  int growValueListBy;

  /* Get item and if not found, bail */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem == NULL)  return FALSE;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled && !configurationBeingInstalled) return FALSE;

  /* See if we hit the limit yet */
  if (theItem->valueCount >= theItem->maxValueCount) return FALSE;

  /* Make sure there is a free item */
  if (theItem->valueCount == theItem->valueAllocCount) {
    if (theItem->maxValueCount == 1)
      growValueListBy = 1;
    else {
      growValueListBy= (theItem->maxValueCount / 2);
      if (growValueListBy > 8) growValueListBy = 8;
    }

    theItem->valueAllocCount += growValueListBy;
    theItem->valueList = realloc(theItem->valueList, sizeof(String) * theItem->valueAllocCount);
  }

  theItem->valueList[theItem->valueCount++] = xPL_StrDup(itemValue);
  return TRUE;
}

/* Set a item value at a given index.  If that index is above    */
/* the actual number of values, the value is appeneded (i.e. may */
/* not be the same index as passed                               */
void xPL_setServiceConfigValueAt(xPL_ServicePtr theService, String itemName, int valueIndex, String itemValue) {
  /* Locate the item */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem == NULL) return;

  /* No changes to list of configurables while the service is enabled */
  if (theService->serviceEnabled) return;

  /* See if this is an add or set */
  if (valueIndex < theItem->valueCount) {
    STR_FREE(theItem->valueList[valueIndex]);
    theItem->valueList[valueIndex] = xPL_StrDup(itemValue);
    return;
  }

  /* Add this */
  xPL_addServiceConfigValue(theService, itemName, itemValue);
}

/* Simple form to set first/only value in an item */
void xPL_setServiceConfigValue(xPL_ServicePtr theService, String itemName, String itemValue) {
  xPL_setServiceConfigValueAt(theService, itemName, 0, itemValue);
}

/* Return the number of values for a given configurable */
int xPL_getServiceConfigValueCount(xPL_ServicePtr theService, String itemName) {
  /* Get item and if not found, bail */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem == NULL) return 0;
  return theItem->valueCount;
}

/* Return the value at the given index.  If the value is NULL of the */
/* index is out of range, NULL is returned                           */
String xPL_getServiceConfigValueAt(xPL_ServicePtr theService, String itemName, int valueIndex) {
  /* Locate the item */
  xPL_ServiceConfigurablePtr theItem = xPL_findServiceConfigurable(theService, itemName);
  if (theItem == NULL) return NULL;

  /* Make sure it's in range */
  if (valueIndex >= theItem->valueCount) return NULL;

  /* Out of range */
  return theItem->valueList[valueIndex];
}

/* Return the value of the first/only index for an item */
String xPL_getServiceConfigValue(xPL_ServicePtr theService, String itemName) {
  return xPL_getServiceConfigValueAt(theService, itemName, 0);
}
