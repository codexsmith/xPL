// xPL Linux Hal Server


#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <time.h>

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
    isTarget = false;

    //Test code here
//    addMember("Wee1", "Poop1");
//    addMember("Wee2", "Poop2");
//    addMember("Wee3", "Poop3");
//    addMember("Wee4", "Poop4");
//    addMember("Wee5", "Poop5");
//    addMember("Wee6", "Poop6");
//
//    string findMe = findMember("Wee1");
//    if (!findMe.empty())
//        cout << "Wee Value: " << findMe << "\n";
}

// XPLMessage Destructor
XPLMessage::~XPLMessage()
{

}

string XPLMessage::findMember(string member)
{
    for (int i = 0; i < members.size(); i++)
    {
        if (member.compare(members[i].member) == 0)
            return members[i].value;
    }
    return "";
}

void XPLMessage::addMember(string member, string value)
{
    XPLValuePair temp;
    temp.member = member; temp.value = value;
    members.push_back(temp);
}

string XPLMessage::getMsgType()
{
    return messageType;
}


void XPLMessage::setMsgType(string messageType)
{
    this->messageType = messageType;
}

XPLAddress XPLMessage::getSource()
{
    return source;
}

void XPLMessage::setSource(string vendor, string device, string instance)
{
    source.vendor = vendor;
    source.device = device;
    source.instance = instance;
}

XPLAddress XPLMessage::getDestination()
{
    return destination;
}

void XPLMessage::setDestination(string vendor, string device, string instance)
{
    destination.vendor = vendor;
    destination.device = device;
    destination.instance = instance;

    //destination exists
    isTarget = true;
}

int XPLMessage::getHops()
{
    return hops;
}

void XPLMessage::setHops(int hops)
{
    this->hops = hops;
}
XPLSchema XPLMessage::getSchema()
{
    return schema;
}

void XPLMessage::setSchema(string schemaClass, string schemaType)
{
    schema.schema = schemaClass;
    schema.type = schemaType;
}

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
