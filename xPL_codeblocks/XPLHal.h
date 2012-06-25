// xPL Linux Hal Server

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class XPLAddress
{
    public:
    string vendor, device, instance;
};

class XPLSchema
{
    public:
    string schema, type;
};

class XPLValuePair
{
    public:
    string member, value;
};

class XPLMessage
{
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

	bool isTarget;

//	string destinationVendor;
//	string destinationDevice;
//	string destinationInstance;

    XPLAddress destination;

//	vector<string> members;
//	vector<string> values;
    vector<XPLValuePair> members;

	//Prototypes
    public:

    //Constructor
    XPLMessage();

    //Destructor
    ~XPLMessage();

	//returns value of a specified parameter name
	string findMember(string memberName);

	//adds member to list
	void addMember(string member, string value);

	//returns message type (cmnd, status, trigger)
	string getMsgType();

	//sets message type
	void setMsgType(string messageType);

	//returns source address (vendor-device.instance)
	XPLAddress getSource();

	//sets source address
	void setSource(string sourceVendor, string sourceDevice, string sourceInstance);

	//returns destination/target (vendor-device.instance)
	XPLAddress getDestination();

	//sets destination address
	void setDestination(string destinationVendor, string destinationDevice, string destinationInstance);

	//returns number of hops allowed set in message
	int getHops();

	//sets number of hops allowed
	void setHops(int hops);

	//returns schema (class.type)
	XPLSchema getSchema();

	//sets schema
	void setSchema(string schemaClass, string schemaType);

	//returns struct of all parameters maybe?
	//someStruct getParameters();

	//creates decoupled copy of message object
	XPLMessage copyMessage();
};
