#ifndef XPLMessage_H
#define XPLMessage_H

#include "XPLHal.h"

using namespace std;

class XPLMessage
{
	public:
	    XPLMessage();
	    ~XPLMessage();

		string findMember(string memberName);
	    vector<XPLValuePair> getMembers();
		void addMember(string member, string value);

		string getMsgType();
		void setMsgType(string messageType);

		XPLAddress getSource();
		void setSource(XPLAddress address);
		void setSource(string sourceVendor, string sourceDevice, string sourceInstance);

	    bool isBroadcast();
	    void setBroadcast(bool broadcast);

		XPLAddress getDestination();
		void setDestination(XPLAddress address);
		void setDestination(string destinationVendor, string destinationDevice, string destinationInstance);

		int getHops();
		void setHops(int hops);

		XPLSchema getSchema();
		void setSchema(XPLSchema schema);
		void setSchema(string schemaClass, string schemaType);

		XPLMessage copyMessage();

	private:
	    string timeReceived;
		string messageType;
		int hops;
	    XPLSchema schema;

	    XPLAddress source;
	    XPLAddress destination;
	    bool broadcast;
	    vector<XPLValuePair> members;
};

#endif // XPLMessage_H
