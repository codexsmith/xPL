
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "XPLParser.h"
#include "XPLMessage.h"
#include "XPLRuleManager.h"


extern XPLRuleManager* ruleMgr;

XPLParser::XPLParser()
{
}

char* uc(string str)
{
    char *newChar=new char[str.size()+1];
    newChar[str.size()]=0;
    memcpy(newChar,str.c_str(),str.size());

    return newChar;
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
    if (msg.isBroadcast())
    {
        if ((theMessage = xPL_createBroadcastMessage(theService, msgType)) == NULL) {
            fprintf(stderr, "Unable to create broadcast message\n");
            return FALSE;
        }
    }
    else
    {
        if ((theMessage = xPL_createTargetedMessage(theService, msgType, uc(msg.getDestination().vendor),
                uc(msg.getDestination().device), uc(msg.getDestination().instance))) == NULL) {
            fprintf(stderr, "Unable to create targeted message\n");
            return FALSE;
        }
    }

    //Setup the schema in the message
    char *schemaType=new char[msg.getSchema().type.size()+1];
    schemaType[msg.getSchema().type.size()]=0;
    memcpy(schemaType,msg.getSchema().type.c_str(),msg.getSchema().type.size());

    char *schemaClass=new char[msg.getSchema().schema.size()+1];
    schemaClass[msg.getSchema().schema.size()]=0;
    memcpy(schemaClass,msg.getSchema().schema.c_str(),msg.getSchema().schema.size());

    xPL_setSchema(theMessage, schemaClass, schemaType);

    //Add members to the message
    for (int i = 0; i < members.size(); i++)
    {
        xPL_addMessageNamedValue(theMessage, uc(members[i].member), uc(members[i].value));
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
    
    syslog(LOG_DEBUG , "Received an XPL message: ");
    
    
    msg.setSchema(xPL_getSchemaClass(theMessage), xPL_getSchemaType(theMessage));
    
    memberList = xPL_getMessageBody(theMessage);
    for (int i = 0; i < xPL_getNamedValueCount(memberList); i++)
    {
        member = xPL_getNamedValuePairAt(memberList, i);
        msg.addMember(member->itemName, member->itemValue);
        string toprint = member->itemName;
        toprint += " ";
        toprint += member->itemValue;
        syslog(LOG_DEBUG , toprint.c_str() );
    }

    /////////// ADD CODE HERE TO PASS MESSAGE TO RULE MANAGER /////////////////
    vector<XPLMessage> messagesToSend = ruleMgr->match(msg);

    string toprintd = "dest:";
    toprintd += msg.getDestination().vendor;
    toprintd += " ";
    toprintd += msg.getDestination().device;
    toprintd += " ";
    toprintd += msg.getDestination().instance;
    syslog(LOG_DEBUG , toprintd.c_str() );
    
    
    
    for (int i = 0; i < messagesToSend.size(); i++)
    {
        sendMsg(messagesToSend[i]);
    }
    ///////////////////////////////////////////////////////////////////////////
}
