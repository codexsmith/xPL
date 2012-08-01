#include "../XPLAction.h"
#include "../XPLMessage.h"
#include <vector>
#include <iostream>
#include <string>

class XPLActionTest {
public:

	bool compareValuePair(XPLValuePair pairOne, XPLValuePair pairTwo)
	{
		bool memberEqual = pairOne.member.compare(pairTwo.member) == 0;
		bool valueEqual = pairOne.value.compare(pairTwo.value) == 0;
		return memberEqual && valueEqual;
	}

	bool compareXPLMessage(XPLMessage* messageOne, XPLMessage* messageTwo)
	{
	    bool msgTypeEqual = (messageOne->getMsgType().compare(messageTwo->getMsgType()) == 0);
	    XPLAddress destinationOne = messageOne->getDestination();
	    XPLAddress destinationTwo = messageTwo->getDestination();
	    bool destinationEqual = (destinationOne.vendor.compare(destinationTwo.vendor) == 0) && (destinationOne.device.compare(destinationTwo.device) == 0) && (destinationOne.instance.compare(destinationTwo.instance) == 0);
	    XPLAddress sourceOne = messageOne->getSource();
	    XPLAddress sourceTwo = messageTwo->getSource();
	    bool sourceEqual = (sourceOne.vendor.compare(sourceTwo.vendor) == 0) && (sourceOne.device.compare(sourceTwo.device) == 0) && (sourceOne.instance.compare(sourceTwo.instance) == 0);
	    bool hopsEqual = messageOne->getHops() == messageTwo->getHops();
	    XPLSchema schemaOne = messageOne->getSchema();
	    XPLSchema schemaTwo = messageTwo->getSchema();
	    bool schemaEqual = (schemaOne.schema.compare(schemaTwo.schema) == 0) && (schemaOne.type.compare(schemaTwo.type) == 0);
	    vector<XPLValuePair> pairsOne = messageOne->getMembers();
	    vector<XPLValuePair> pairsTwo = messageOne->getMembers();
	    bool pairsEqual = pairsOne.size() == pairsTwo.size();
	    if(pairsEqual)
	    {
	    	int i = 0;
	    	while(pairsEqual && i<pairsOne.size())
	    	{
	    		pairsEqual = compareValuePair(pairsOne[i], pairsTwo[i]);
	    		i++;
	    	}
	    }

	    return msgTypeEqual && destinationEqual && sourceEqual && hopsEqual && schemaEqual && pairsEqual;
	}

	void testExecute()
	{
		XPLMessage messageOne, messageTwo;

		messageOne.addMember("firstResponseMemberOne", "firstResponeValueOne");
		messageOne.addMember("firstResponseMemberTwo", "firstResponseValueTwo");
		messageOne.setMsgType("firstMessageType");
		messageOne.setBroadcast(false);
		messageOne.setSchema("firstResponseSchemaClass", "firstResponseSchemaType");
		messageOne.setHops(2);
		messageOne.setSource("messageOneVendor", "messageOneDevice", "messageOneInstance");
		messageOne.setDestination("messageOneDestinationVendor", "messageTwoDestinationDevice", "messageOneDestinationInstance");

		messageTwo.addMember("firstResponseMemberOne", "firstResponeValueOne");
		messageTwo.addMember("firstResponseMemberTwo", "firstResponseValueTwo");
		messageTwo.setMsgType("firstMessageType");
		messageTwo.setBroadcast(false);
		messageTwo.setSchema("firstResponseSchemaClass", "firstResponseSchemaType");
		messageTwo.setHops(2);
		messageTwo.setSource("messageOneVendor", "messageOneDevice", "messageOneInstance");
		messageTwo.setDestination("messageOneDestinationVendor", "messageTwoDestinationDevice", "messageOneDestinationInstance");

		vector<XPLMessage>* actionVector = new vector<XPLMessage>();
		actionVector->push_back(messageOne);

		XPLAction action(actionVector);
		vector<XPLMessage> messages = action.execute();

		bool result = compareXPLMessage(&messages[0], &messageTwo);
		if(result)
			cout << "Test XPLAction::execute() success!\n";
		else
			cout << "Test XPLAction::execute() failed!\n";

	}

	void runTests()
	{
		testExecute();
	}
};
