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
	messageOne.setBroadcast(true);
	messageOne.setSchema("firstResponseClass", "firstResponseType");	

	return 0;
}
