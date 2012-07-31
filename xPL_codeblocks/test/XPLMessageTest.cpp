#include "../XPLHal.h"
#include "../XPLMessage.h"
#include <vector>
#include <iostream>
#include <string>

class XPLMessageTest {
public:

	static bool compareValuePair(XPLValuePair pairOne, XPLValuePair pairTwo)
	{
		bool memberEqual = pairOne.member.compare(pairTwo.member) == 0;
		bool valueEqual = pairOne.value.compare(pairTwo.value) == 0;
		return memberEqual && valueEqual;
	}

	static bool compareXPLMessage(XPLMessage* messageOne, XPLMessage* messageTwo)
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

	static void testExecute()
	{
		XPLMessage messageOne, messageTwo;
		bool result;
		bool finalResult = true;

		messageOne.setMsgType("firstMessageType");
		messageOne.setBroadcast(false);
		messageOne.setSchema("firstMessageSchemaClass", "firstMessageSchemaType");
		messageOne.setHops(2);
		messageOne.setSource("firstMessageSourceVendor", "firstMessageSourceDevice", "firstMessageSourceInstance");
		messageOne.setDestination("firstMessageDestinationVendor", "firstMessageDestinationDevice", "firstMessageDestinationInstance");
		messageOne.addMember("firstMessageMemberOne", "firstMessageValueOne");
		messageOne.addMember("firstMessageMemberTwo", "firstMessageValueTwo");

        //Test message was created properly
		result = messageOne.getMsgType().compare("firstMessageType") == 0;
		if(result)
			cout << "Test XPLMessage integrity (msgType) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (msgType) failed!\n";
			finalResult = false;
		}

		result = messageOne.isBroadcast() == false;
		if(result)
			cout << "Test XPLMessage integrity (broadcast) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (broadcast) failed!\n";
			finalResult = false;
		}

        XPLSchema s = messageOne.getSchema();
		result = s.schema.compare("firstMessageSchemaClass") == 0 && s.type.compare("firstMessageSchemaType") == 0;
		if(result)
			cout << "Test XPLMessage integrity (schema) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (schema) failed!\n";
			finalResult = false;
		}

		result = messageOne.getHops() == 2;
		if(result)
			cout << "Test XPLMessage integrity (hops) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (hops) failed!\n";
			finalResult = false;
		}

        XPLAddress sa = messageOne.getSource();
		result = sa.vendor.compare("firstMessageSourceVendor") == 0 && sa.device.compare("firstMessageSourceDevice") == 0
                 && sa.instance.compare("firstMessageSourceInstance") == 0;
		if(result)
			cout << "Test XPLMessage integrity (sourceAddress) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (sourceAddress) failed!\n";
			finalResult = false;
		}

        XPLAddress da = messageOne.getDestination();
		result = da.vendor.compare("firstMessageDestinationVendor") == 0 && da.device.compare("firstMessageDestinationDevice") == 0
                 && da.instance.compare("firstMessageDestinationInstance") == 0;
		if(result)
			cout << "Test XPLMessage integrity (destinationAddress) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (destinationAddress) failed!\n";
			finalResult = false;
		}

		result = messageOne.findMember("firstMessageMemberOne").compare("firstMessageValueOne") == 0 &&
		         messageOne.findMember("firstMessageMemberTwo").compare("firstMessageValueTwo") == 0;
		if(result)
			cout << "Test XPLMessage integrity (members) success!\n";
		else
		{
			cout << "Test XPLMessage integrity (members) failed!\n";
			finalResult = false;
		}

        //Display final result
		if(finalResult)
			cout << "\nTest XPLMessage::execute() success!\n";
		else
			cout << "\nTest XPLMessage::execute() failed!\n";
	}

	static void runTests()
	{
		testExecute();
	}
};
