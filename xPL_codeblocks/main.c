// xPL Linux Hal Server

#include <stdio.h>
#include "xPLLib/xPL.h"

int main(int argc, String argv[])
{
    int argIndex = 0;
    xPL_ServicePtr theService = NULL;
    xPL_MessagePtr theMessage = NULL;
    String delimPtr;

    //Source Address
    String srcVendor = "Test";
    String srcDeviceID = "test";
    String srcInstanceID = "test";

    //Message Structure
    xPL_MessageType msgType = xPL_MESSAGE_COMMAND;
    String msgSchemaClass = "config";
    String msgSchemaType = "basic";
    //String msgSchemaClass = NULL;
    //String msgSchemaType = NULL;

    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL");
        exit(1);
    }

    /* Create service so we can create messages */
    if ((theService = xPL_createService(srcVendor, srcDeviceID, srcInstanceID)) == NULL) {
        fprintf(stderr, "Unable to create xPL service\n");
        return FALSE;
    }

    if ((theMessage = xPL_createBroadcastMessage(theService, msgType)) == NULL) {
        fprintf(stderr, "Unable to create broadcast message\n");
        return FALSE;
    }

    /* Install the schema */
    printf("(msgSchemaClass, msgSchemaType) = (%s, %s)\n\n", msgSchemaClass, msgSchemaType);
    xPL_setSchema(theMessage, msgSchemaClass, msgSchemaType);

    /* Install named values */
    xPL_addMessageNamedValue(theMessage, "lightlevel", "70");

    /* Send the message */
    if (!xPL_sendMessage(theMessage)) {
        fprintf(stderr, "Unable to send xPL message\n");
        return FALSE;
    }

    printf("Message Sent!!\n");

    return TRUE;
}
