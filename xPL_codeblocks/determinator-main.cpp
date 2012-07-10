// xPL Linux Hal Server
#define HAL_VERSION "1.0"

#include <string>
#include <stdio.h>
#include "XPLHal.h";
#include "XPLMessage.h"
#include "XPLParser.h"
#include "XPLRuleManager.h"
#include "Determinator.h"
#include "XPLAction.h"
#include "XPLCondition.h"

extern "C" {

    #include "xPLLib/xPL.h"

}
//#include <boost/asio.hpp>

xPL_ServicePtr theService = NULL;
XPLRuleManager* ruleMgr;

Determinator* createDeterminator()
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
	
	Determinator* determinator = new Determinator(condition, action);
	return determinator;
}

int main(int argc, String argv[])
{
    /**** Test Code ****/
    XPLMessage testMsg, testMsg2;
    testMsg.addMember("Fruit", "Dealer");
    testMsg.addMember("SK", "MC");
    testMsg.setHops(5);
    testMsg.setSource("Pin", "Tin", "Sin");
    testMsg.setMsgType("Command");

    testMsg2 = testMsg.copyMessage();
    testMsg2.addMember("StarTale", "July");
    testMsg2.setMsgType("Status");

    testMsg.setSource("Poop", "Scoop", "Loop");
    testMsg.setHops(8);

    //Load XML document

    //Initialize RuleManager object with returned vector of determinators
	vector<Determinator>* determinators = new vector<Determinator>();
	Determinator* determinator = createDeterminator();
	determinators->push_back(determinator);

	ruleMgr = new XPLRuleManager(determinators);

    //Source Address
    String srcVendor = "Test";
    String srcDeviceID = "hal";
    String srcInstanceID = "322F";

    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL");
        exit(1);
    }

    /* And a listener for all xPL messages */
    xPL_addMessageListener(XPLParser::recvMsg, NULL);

    /* Create a service so the hubs know to send things to us        */
    /* While we are not relaly using he service, xPL hubs will not   */
    /* forward messages to us until they have seen a xPL-looking     */
    /* device on the end of a hub connection, so this just gets us a */
    /* place at the table, so to speak                               */
    theService = xPL_createConfigurableService(srcVendor, srcDeviceID, "Test.hal");
    xPL_setServiceVersion(theService, HAL_VERSION);

    xPL_setServiceEnabled(theService, TRUE);

    /* Hand control over to xPLLib */
    xPL_processMessages(-1);

    return TRUE;
}
