// xPL Linux Hal Server
// jon adding comment, i know, I'm lame.
#include <stdio.h>
#include "XPLHal.h";
extern "C" {

    #include "xPLLib/xPL.h"

}

int main(int argc, String argv[])
{
    XPLMessage testMsg, testMsg2;
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
    String msgSchemaClass = "bull";
    String msgSchemaType = "crap";
    //String msgSchemaClass = NULL;
    //String msgSchemaType = NULL;


    /**** Test Code ****/
    testMsg.addMember("Fruit", "Dealer");
    testMsg.addMember("SK", "MC");
    testMsg.setHops(5);
    testMsg.setSource("Pin", "Tin", "Sin");
    testMsg.setMsgType("Command");

    testMsg2 = testMsg.copyMessage();
    testMsg2.addMember("StarTale", "July");
    testMsg2.setMsgType("Status");

    testMsg.setSource("Poop", "Scoop", "Loop");
    testMsg.setHops(8);

    cout << "------Comparing Message created through copyMessage() function -------\n";
    cout << testMsg2.getSource().device << " -- " << testMsg.getSource().device << "\n";
    cout << testMsg2.getHops() << " -- " << testMsg.getHops() << "\n";
    cout << testMsg2.getMsgType() << " -- " << testMsg.getMsgType() << "\n";

    if (!testMsg.findMember("StarTale").empty())
        cout << "Whoops: " << testMsg.findMember("StarTale") << "\n";
    if (!testMsg2.findMember("StarTale").empty())
        cout << "Yay: " << testMsg2.findMember("StarTale") << "\n";

    cout << "-----------------------------------------------------------------------\n\n";

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
