// xPL Linux Hal Server
#define HAL_VERSION "1.0"

#include <stdio.h>
#include "XPLHal.h";
#include "XPLMessage.h";

extern "C" {

    #include "xPLLib/xPL.h"

}
//#include <boost/asio.hpp>

//Prototypes
int sendMsg(xPL_ServicePtr xPLService, int command);
void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue);

static xPL_ServicePtr theService = NULL;

int sendMsg(xPL_ServicePtr xPLService, int command)
{
    int delays[] = {600, 300, 600, 200, 600, 100, 600, 100, 600, 100, 600, 100, 600, 100, 600, 300, 600, 200, 600, 100, 600, 100, 600, 100, 600, 100, 600, 100, 50};
    int delaysSize = sizeof(delays) / sizeof(int);
    int x = 0;
    xPL_MessagePtr theMessage = NULL;
    String delimPtr;
    //Message Structure
    xPL_MessageType msgType = xPL_MESSAGE_COMMAND;
    String msgSchemaClass = "control";
    String msgSchemaType = "basic";

    if ((theMessage = xPL_createTargetedMessage(xPLService, msgType, "smgpoe", "lamp", "1")) == NULL) {
        fprintf(stderr, "Unable to create broadcast message\n");
        return FALSE;
    }

    /* Install the schema */
    printf("(msgSchemaClass, msgSchemaType) = (%s, %s)\n\n", msgSchemaClass, msgSchemaType);
    xPL_setSchema(theMessage, msgSchemaClass, msgSchemaType);

    if (command==0)
    {
        /* Install named values */
        xPL_addMessageNamedValue(theMessage, "device", "pwm");
        xPL_addMessageNamedValue(theMessage, "type", "variable");
        xPL_addMessageNamedValue(theMessage, "current", "0");

        /* Send the message */
        if (!xPL_sendMessage(theMessage)) {
            fprintf(stderr, "Unable to send xPL message\n");
            return FALSE;
        }
    }
    else
    {
        for (int i = 0; i < delaysSize; i++)
        {
            //Create a new message each iteration
            if ((theMessage = xPL_createTargetedMessage(xPLService, msgType, "smgpoe", "lamp", "1")) == NULL) {
                fprintf(stderr, "Unable to create broadcast message\n");
                return FALSE;
            }
            /* Install the schema */
            printf("(msgSchemaClass, msgSchemaType) = (%s, %s)\n\n", msgSchemaClass, msgSchemaType);
            xPL_setSchema(theMessage, msgSchemaClass, msgSchemaType);

            /* Install named values */
            if (x == 0)
            {
                printf("test\n");
                xPL_addMessageNamedValue(theMessage, "device", "pwm");
                xPL_addMessageNamedValue(theMessage, "type", "variable");
                xPL_addMessageNamedValue(theMessage, "current", "50");
            }
            else
            {
                printf("test2\n");
                xPL_addMessageNamedValue(theMessage, "device", "pwm");
                xPL_addMessageNamedValue(theMessage, "type", "variable");
                xPL_addMessageNamedValue(theMessage, "current", "0");
            }

            /* Send the message */
            if (!xPL_sendMessage(theMessage)) {
                fprintf(stderr, "Unable to send xPL message\n");
                return FALSE;
            }
            printf("%d\n", x);
            x = (x+1)%2;
            usleep(1000*delays[i]);
        }
    }

    printf("Message Sent!!\n");
}

void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue)
{
    //We should see this!!
    printf("Receiving a Message\n");

    if (strcmp(xPL_getSourceVendor(theMessage), "smgpoe") == 0 &&
        strcmp(xPL_getSourceDeviceID(theMessage), "lamp") == 0 &&
        strcmp(xPL_getSourceInstanceID(theMessage), "3") == 0)
    {
        if (xPL_doesMessageNamedValueExist(theMessage, "device") && strcmp(xPL_getMessageNamedValue(theMessage, "device"), "button2") == 0)
        {
            if (xPL_doesMessageNamedValueExist(theMessage, "current") && strcmp(xPL_getMessageNamedValue(theMessage, "current"), "HIGH") == 0)
            {
                if (!sendMsg(theService, 1))
                {
                    fprintf(stderr, "Unable to send xPL message.");
                    exit(1);
                }
            }
        }
    }

    if (strcmp(xPL_getSourceVendor(theMessage), "smgpoe") == 0 &&
        strcmp(xPL_getSourceDeviceID(theMessage), "lamp") == 0 &&
        strcmp(xPL_getSourceInstanceID(theMessage), "3") == 0)
    {
        String temp1 = "device";
        String temp2 = "button2";
        if (xPL_doesMessageNamedValueExist(theMessage, "device") && strcmp(xPL_getMessageNamedValue(theMessage, "device"), "button1") == 0)
        {
            if (xPL_doesMessageNamedValueExist(theMessage, "current") && strcmp(xPL_getMessageNamedValue(theMessage, "current"), "HIGH") == 0)
            {
                if (!sendMsg(theService, 0))
                {
                    fprintf(stderr, "Unable to send xPL message.");
                    exit(1);
                }
            }
        }
    }
}

int main(int argc, String argv[])
{
    /**** Test Code ****/
    XPLMessage testMsg, testMsg2;
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


    //Source Address
    String srcVendor = "Test";
    String srcDeviceID = "hal";
    String srcInstanceID = "322F";

    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL");
        exit(1);
    }

    /* And a listener for all xPL messages */
    xPL_addMessageListener(recvMsg, NULL);

    /* Create a service so the hubs know to send things to us        */
    /* While we are not relaly using he service, xPL hubs will not   */
    /* forward messages to us until they have seen a xPL-looking     */
    /* device on the end of a hub connection, so this just gets us a */
    /* place at the table, so to speak                               */
    theService = xPL_createConfigurableService(srcVendor, srcDeviceID, "Test.hal");
    xPL_setServiceVersion(theService, HAL_VERSION);

    xPL_setServiceEnabled(theService, TRUE);

    /* Hand control over to xPLLib */
    xPL_processMessages(-1);

    return TRUE;
}
