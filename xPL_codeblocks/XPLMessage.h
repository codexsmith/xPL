#ifndef XPLMessage_H
#define XPLMessage_H

#include "XPLHal.h"

using namespace std;

class XPLMessage
{
    //PUBLIC//
    public:

	//FUNCTION PROTOTYPES

    //Constructor
    XPLMessage();

    //Destructor
    ~XPLMessage();

	//returns value of a specified parameter name
	string findMember(string memberName);

	//returns entire vector of members
    vector<XPLValuePair> getMembers();

	//adds member to list
	void addMember(string member, string value);

	//returns message type (cmnd, status, trigger)
	string getMsgType();

	//sets message type
	void setMsgType(string messageType);

	//returns source address (vendor-device.instance)
	XPLAddress getSource();

	//sets source address
	void setSource(XPLAddress address);
	void setSource(string sourceVendor, string sourceDevice, string sourceInstance);

    //returns wether or not the message was a broadcast
    bool isBroadcast();

    //sets the broadcast flag
    void setBroadcast(bool broadcast);

	//returns destination/target (vendor-device.instance)
	XPLAddress getDestination();

	//sets destination address
	void setDestination(XPLAddress address);
	void setDestination(string destinationVendor, string destinationDevice, string destinationInstance);

	//returns number of hops allowed set in message
	int getHops();

	//sets number of hops allowed
	void setHops(int hops);

	//returns schema (class.type)
	XPLSchema getSchema();

	//sets schema
	void setSchema(XPLSchema schema);
	void setSchema(string schemaClass, string schemaType);

	//returns struct of all parameters maybe?
	//someStruct getParameters();

	//creates decoupled copy of message object
	XPLMessage copyMessage();

	//PRIVATE//
    private:

    //VARIABLES

	//Statistics
    string timeReceived;

	//Message Data
	string messageType;
	int hops;

//	string schemaClass;
//	string schemaType;

    XPLSchema schema;

//	string sourceVendor;
//	string sourceDevice;
//	string sourceInstance;

    XPLAddress source;

	bool broadcast;

//	string destinationVendor;
//	string destinationDevice;
//	string destinationInstance;

    XPLAddress destination;

//	vector<string> members;
//	vector<string> values;
    vector<XPLValuePair> members;
};

#endif // XPLMessage_H
