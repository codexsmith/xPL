// xPL Linux Hal Server


#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>
#include <syslog.h>

#include "XPLHal.h"
#include "XPLMessage.h"

using namespace std;

// XPLMessage Constructor
XPLMessage::XPLMessage()
{
    time_t seconds = time(NULL);
    struct tm *time = localtime(&seconds);
    timeReceived = asctime(time);

    //initialize target boolean
    broadcast = true;
}

// XPLMessage Destructor
XPLMessage::~XPLMessage()
{

}

//returns value of a specified parameter name
string XPLMessage::findMember(string member)
{
    for (int i = 0; i < members.size(); i++)
    {
        if (member.compare(members[i].member) == 0)
            return members[i].value;
    }
    return "";
}

//returns entire vector of members
vector<XPLValuePair> XPLMessage::getMembers()
{
    return members;
}

//adds member to list
void XPLMessage::addMember(string member, string value)
{
    XPLValuePair temp;
    temp.member = member; temp.value = value;
    members.push_back(temp);
}

//returns message type (cmnd, status, trigger)
string XPLMessage::getMsgType()
{
    return messageType;
}

//sets message type
void XPLMessage::setMsgType(string messageType)
{
    this->messageType = messageType;
}

//returns source address (vendor-device.instance)
XPLAddress XPLMessage::getSource()
{
    return source;
}

//sets source address
void XPLMessage::setSource(XPLAddress address)
{
    source = address;
}

//sets source address
void XPLMessage::setSource(string vendor, string device, string instance)
{
    source.vendor = vendor;
    source.device = device;
    source.instance = instance;
}

//returns destination/target (vendor-device.instance)
XPLAddress XPLMessage::getDestination()
{
    return destination;
}

//sets destination address
void XPLMessage::setDestination(XPLAddress address)
{
    destination = address;
}

//sets destination address
void XPLMessage::setDestination(string vendor, string device, string instance)
{
    destination.vendor = vendor;
    destination.device = device;
    destination.instance = instance;

    //destination exists
    broadcast = false;
}

//returns number of hops allowed set in message
int XPLMessage::getHops()
{
    return hops;
}

//sets number of hops allowed
void XPLMessage::setHops(int hops)
{
    this->hops = hops;
}

//returns schema (class.type)
XPLSchema XPLMessage::getSchema()
{
    return schema;
}

//sets schema
void XPLMessage::setSchema(XPLSchema schema)
{
    this->schema = schema;
}

//sets schema
void XPLMessage::setSchema(string schemaClass, string schemaType)
{
    schema.schema = schemaClass;
    schema.type = schemaType;
}

//creates decoupled copy of message object
XPLMessage XPLMessage::copyMessage()
{
    XPLMessage msg;

    for (int i = 0; i < members.size(); i++)
        msg.addMember(members[i].member, members[i].value);

    msg.setMsgType(messageType);

    msg.setSource(source.vendor, source.device, source.instance);

    msg.setDestination(destination.vendor, destination.device, destination.instance);

    msg.setHops(hops);

    msg.setSchema(schema.schema, schema.type);

    return msg;
}

//returns wether or not the message was a broadcast
bool XPLMessage::isBroadcast()
{
    return broadcast;
}

//sets the broadcast flag
void XPLMessage::setBroadcast(bool broadcast)
{
    this->broadcast = broadcast;
}

void XPLMessage::sendToSyslog()
{
    syslog(LOG_DEBUG , ("Source: " + source.vendor + "-" + source.device + "." + source.instance).c_str());
    syslog(LOG_DEBUG ,  ("Destination: " + destination.vendor + "-" + destination.device + "." + destination.instance ).c_str());
    syslog(LOG_DEBUG , ( "Message Type: " + messageType ).c_str());
    syslog(LOG_DEBUG , ("Schema: " + schema.schema + "." + schema.type).c_str());
    syslog(LOG_DEBUG , "Members: ");
    
    vector<XPLValuePair>::iterator it;
    for ( it=members.begin() ; it!=members.end(); ++it )
    {
        syslog(LOG_DEBUG ,  ((*it).member + " = " + (*it).value).c_str());
    }
     
}
