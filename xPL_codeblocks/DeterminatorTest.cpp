#include "Determinator.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLHal.h"

#include <string>
#include <iostream>

int main()
{
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

	XPLCondition* condition = new XPLCondition(conditionVector);

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

	XPLAction* action = new XPLAction(actionVector);
	
	Determinator determinator(condition, action);
	if(determinator.match(&messageOne))
		cout << "It matches!\n";
	else
		cout << "Message didn't match\n";

	XPLMessage triggerMessage;
	triggerMessage.addMember(pairOne.member, pairOne.value);
	triggerMessage.addMember(pairTwo.member, pairTwo.value);
	triggerMessage.addMember(pairThree.member, pairThree.value);
		
	if(determinator.match(&triggerMessage))
		cout << "It matches!\n";
	else
		cout << "Message didn't match\n";

	vector<XPLMessage> responses = determinator.execute();
	if(responses.size() == 2)
		cout << "I probably got the same two messages back\n";	
	else
		cout << "You done goofed";

	return 0;
}
