
#include <stdio.h>
#include <stdlib.h>
#include "XPLParser.h"
#include "XPLMessage.h"




int level = 0;
extern xPL_ServicePtr  theService;

XPLParser::XPLParser()
{
}

int XPLParser::sendMsg(XPLMessage msg)
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

void XPLParser::recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue)
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

        ////////// PLACE TEST CODE HERE ///////////////////////////////////////////

    char temp[50];

    if (msg.getSource().vendor.compare("smgpoe") == 0 &&
        msg.getSource().device.compare("lamp") == 0 &&
        msg.getSource().instance.compare("3") == 0 &&
        msg.findMember("device").compare("button2") == 0 &&
        msg.findMember("current").compare("HIGH") == 0)
    {
        if (level < 60)
            level += 10;

        if (level < 0)
            level = 0;
        else if (level > 60)
            level = 60;

        sprintf(temp,"%d",level);

        XPLMessage turnLampOn;
        turnLampOn.setMsgType("xpl-cmnd");
        turnLampOn.setSource("XPLHal", "XPLHal", "XPLHal");
        turnLampOn.setDestination("smgpoe", "lamp", "1");
        turnLampOn.setSchema("control", "basic");
        turnLampOn.setHops(5);
        turnLampOn.setBroadcast(false);
        turnLampOn.addMember("device", "pwm");
        turnLampOn.addMember("type", "variable");
        turnLampOn.addMember("current", temp);
        sendMsg(turnLampOn);

        turnLampOn.setDestination("smgpoe", "lamp", "3");
        sendMsg(turnLampOn);
    }

    if (msg.getSource().vendor.compare("smgpoe") == 0 &&
        msg.getSource().device.compare("lamp") == 0 &&
        msg.getSource().instance.compare("3") == 0 &&
        msg.findMember("device").compare("button1") == 0 &&
        msg.findMember("current").compare("HIGH") == 0)
    {
        if (level >= 10)
            level -= 10;

        if (level < 0)
            level = 0;
        else if (level > 60)
            level = 60;

        sprintf(temp,"%d",level);

        XPLMessage turnLampOn;
        turnLampOn.setMsgType("xpl-cmnd");
        turnLampOn.setSource("XPLHal", "XPLHal", "XPLHal");
        turnLampOn.setDestination("smgpoe", "lamp", "1");
        turnLampOn.setSchema("control", "basic");
        turnLampOn.setHops(5);
        turnLampOn.setBroadcast(false);
        turnLampOn.addMember("device", "pwm");
        turnLampOn.addMember("type", "variable");
        turnLampOn.addMember("current", temp);
        sendMsg(turnLampOn);

        turnLampOn.setDestination("smgpoe", "lamp", "3");
        sendMsg(turnLampOn);
    }

    ///////////////////////////////////////////////////////////////////////////
}
