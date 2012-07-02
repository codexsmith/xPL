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
int sendMsg(XPLMessage msg);
void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue);

static xPL_ServicePtr theService = NULL;

int sendMsg(XPLMessage msg)
{
    vector<XPLValuePair> members = msg.getMembers();

    //xpllib message ptr
    xPL_MessagePtr theMessage = NULL;

    //Message type for old xpllib object
    xPL_MessageType msgType = xPL_MESSAGE_COMMAND;

    //Set the message type
    if (msg.getMsgType().compare("xpl-cmnd") == 0)
        msgType = xPL_MESSAGE_COMMAND;
    else if (msg.getMsgType().compare("xpl-stat") == 0)
        msgType = xPL_MESSAGE_STATUS;
    else if (msg.getMsgType().compare("xpl-trig") == 0)
        msgType = xPL_MESSAGE_TRIGGER;
    else
        msgType = xPL_MESSAGE_ANY;

    //Create a new message
//    if ((theMessage = xPL_createTargetedMessage(theService, msgType, "temp",
//            "temp", "temp")) == NULL) {
//        fprintf(stderr, "Unable to create broadcast message\n");
//        return FALSE;
//    }
    if (msg.isBroadcast())
    {
        if ((theMessage = xPL_createBroadcastMessage(theService, msgType)) == NULL) {
            fprintf(stderr, "Unable to create broadcast message\n");
            return FALSE;
        }
    }
    else
    {
        if ((theMessage = xPL_createTargetedMessage(theService, msgType, msg.getDestination().vendor.c_str(),
                msg.getDestination().device.c_str(), msg.getDestination().instance.c_str())) == NULL) {
            fprintf(stderr, "Unable to create targeted message\n");
            return FALSE;
        }
    }

    //Setup the schema in the message
    const char* schemaClass = msg.getSchema().schema.c_str();
    const char* schemaType = msg.getSchema().type.c_str();
    xPL_setSchema(theMessage, schemaClass, schemaType);

    //Add members to the message
    for (int i = 0; i < members.size(); i++)
    {
        xPL_addMessageNamedValue(theMessage, members[i].member.c_str(), members[i].value.c_str());
    }

    //Send the message
    if (!xPL_sendMessage(theMessage)) {
        fprintf(stderr, "Unable to send xPL message\n");
        return FALSE;
    }

    usleep(2000);
}

void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue)
{
    XPLMessage msg;
    xPL_NameValueListPtr memberList;
    xPL_NameValuePairPtr member;


    //Set the message type
    switch(xPL_getMessageType(theMessage))
    {
        case xPL_MESSAGE_COMMAND:
            msg.setMsgType("xpl-cmnd");
            break;
        case xPL_MESSAGE_STATUS:
            msg.setMsgType("xpl-stat");
            break;
        case xPL_MESSAGE_TRIGGER:
            msg.setMsgType("xpl-trig");
            break;
        default:
            msg.setMsgType("!UNKNOWN!");
            break;
    }

    //Set the hop count
    msg.setHops(xPL_getHopCount(theMessage));

    //Set the source address
    msg.setSource(xPL_getSourceVendor(theMessage), xPL_getSourceDeviceID(theMessage), xPL_getSourceInstanceID(theMessage));

    //Set the broadcast flag
    if (xPL_isBroadcastMessage(theMessage))
    {
        msg.setBroadcast(true);
    }
    else
    {
        msg.setBroadcast(false);

        //Set the target/destination address
        msg.setDestination(xPL_getTargetVendor(theMessage), xPL_getTargetDeviceID(theMessage), xPL_getTargetInstanceID(theMessage));
    }

    //Set the schema class/type
    msg.setSchema(xPL_getSchemaClass(theMessage), xPL_getSchemaType(theMessage));

    memberList = xPL_getMessageBody(theMessage);
    for (int i = 0; i < xPL_getNamedValueCount(memberList); i++)
    {
        member = xPL_getNamedValuePairAt(memberList, i);
        msg.addMember(member->itemName, member->itemValue);
    }

    /////////// ADD CODE HERE TO PASS MESSAGE TO RULE MANAGER /////////////////
    // XPLMessage sendMe = ruleManager.match(msg);
    // sendMsg(theService, msg);
    ///////////////////////////////////////////////////////////////////////////
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
