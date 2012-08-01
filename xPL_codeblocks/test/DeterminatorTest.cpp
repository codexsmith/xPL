#include "../Determinator.h"
#include "../XPLAction.h"
#include "../XPLCondition.h"
#include "../XPLHal.h"

#include <string>
#include <iostream>

class DeterminatorTest {

public:

	XPLCondition* createCondition()
	{
		XPLAddress sourceAddressOne;
		sourceAddressOne.device = "device";
		sourceAddressOne.vendor = "vendor";
		sourceAddressOne.instance = "instance";
		XPLAddress destinationAddressOne;
		destinationAddressOne.device = "destDevice";
		destinationAddressOne.vendor = "destVendor";
		destinationAddressOne.instance = "destInstance";
		XPLSchema schemaOne;
		schemaOne.schema = "schema";
		schemaOne.type = "type";
		int hopsOne = 5;
		string msgTypeOne = "xpl-cmd";
		//First, let's create the condition
		XPLValuePair pairOne, pairTwo, pairThree;
		pairOne.member = "oneMember";
		pairOne.value = "oneValue";
		pairTwo.member = "twoMember";
		pairTwo.value = "twoValue";
		pairThree.member = "threeMember";
		pairThree.value = "threeValue";

		vector<XPLValuePair>* conditionVector = new vector<XPLValuePair>();
		conditionVector->push_back(pairOne);
		conditionVector->push_back(pairTwo);
		conditionVector->push_back(pairThree);

		XPLCondition* conditionOne = new XPLCondition(conditionVector, sourceAddressOne, destinationAddressOne, schemaOne, hopsOne, msgTypeOne);
		return conditionOne;
	}

	XPLAction* createAction()
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

		messageTwo.addMember("secondResponseMemberOne", "secondResponseValueOne");
		messageTwo.addMember("secondResponseMemberTwo", "secondResponseValueTwo");
		messageTwo.setMsgType("secondMessageType");
		messageTwo.setSchema("secondResponseSchemaClass", "secondResponseSchemaType");
		messageTwo.setBroadcast(true);
		messageTwo.setHops(5);
		messageTwo.setSource("messageTwoSourceVendor", "messageTwoSourceDevice", "messageTwoSourceInstance");

		vector<XPLMessage>* actionVector = new vector<XPLMessage>();
		actionVector->push_back(messageOne);
		actionVector->push_back(messageTwo);

		return new XPLAction(actionVector);
	}

	void testMatch()
	{
		XPLMessage messageOne;

		XPLAddress sourceAddressOne;
		sourceAddressOne.device = "device";
		sourceAddressOne.vendor = "vendor";
		sourceAddressOne.instance = "instance";
		XPLAddress destinationAddressOne;
		destinationAddressOne.device = "destDevice";
		destinationAddressOne.vendor = "destVendor";
		destinationAddressOne.instance = "destInstance";
		XPLSchema schemaOne;
		schemaOne.schema = "schema";
		schemaOne.type = "type";
		int hopsOne = 5;
		string msgTypeOne = "xpl-cmd";

		XPLValuePair pairOne, pairTwo, pairThree;
		pairOne.member = "oneMember";
		pairOne.value = "oneValue";
		pairTwo.member = "twoMember";
		pairTwo.value = "twoValue";
		pairThree.member = "threeMember";
		pairThree.value = "threeValue";	

		vector<XPLValuePair>* conditionVector = new vector<XPLValuePair>();
		conditionVector->push_back(pairOne);
		conditionVector->push_back(pairTwo);
		conditionVector->push_back(pairThree);

		XPLCondition* condition = new XPLCondition(conditionVector, sourceAddressOne, destinationAddressOne, schemaOne, hopsOne, msgTypeOne);
		XPLAction* action = createAction();
		Determinator determinator(condition, action);
		bool failedMatched = determinator.match(&messageOne);
		
		XPLMessage triggerMessage;
		triggerMessage.setSource(sourceAddressOne);
		triggerMessage.setDestination(destinationAddressOne);
		triggerMessage.setHops(hopsOne);
		triggerMessage.setSchema(schemaOne);
		triggerMessage.setMsgType(msgTypeOne);
		triggerMessage.addMember("oneMember", "oneValue");
		triggerMessage.addMember("twoMember", "twoValue");

	
		bool matched = determinator.match(&triggerMessage);
		if(!failedMatched && matched)
			cout << "Test Determinator::match() success!\n";
		else
			cout << "Test Determinator::match() failed!\n";
	}

	void testExecute()
	{
		Determinator determinator(createCondition(), createAction());
		vector<XPLMessage> responses = determinator.execute();
		if(responses.size() == 2)
			cout << "Test Determinator::execute() success!\n";
		else
			cout << "Test Determinator::execute() failed!\n";
	}

	void runTests()
	{
		testMatch();
		testExecute();
	}
};
