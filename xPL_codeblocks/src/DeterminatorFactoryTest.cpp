#include "DeterminatorFactory.h"
#include "XPLHal.h"
#include "Determinator.h"
#include "XPLCondition.h"
#include "XPLMessage.h"
#include "XPLAction.h"

#include <iostream>
#include <string>

class DeterminatorFactoryTest {

public:

	XPLCondition* createXPLCondition()
	{
		XPLAddress sourceAddress;
		sourceAddress.vendor = "";
		sourceAddress.device = "";
		sourceAddress.instance = "";

		XPLAddress destinationAddress;
		destinationAddress.vendor = "";
		destinationAddress.device = "";
		destinationAddress.instance = "";

		XPLSchema schema;
		schema.schema = "";
		schema.type = "";

		int hops = 5;

		string msgType = "xpl-cmd";

		XPLValuePair pairOne, pairTwo, pairThree;
		pairOne.member = "memberOne";
		pairOne.value = "valueOne";
		pairTwo.member = "memberTwo";
		pairTwo.value = "valueTwo";
		pairThree.member = "memberThree";
		pairThree.value = "valueThree"; 

		vector<XPLValuePair>* definitions;
		definitions->push_back(pairOne);
		definitions->push_back(pairTwo);
		definitions->push_back(pairThree);
		XPLCondition* condition = new XPLCondition(definitions, sourceAddress, destinationAddress, schema, hops, msgType);
		return condition;
	}

	bool compareValuePair(XPLValuePair pairOne, XPLValuePair pairTwo)
	{
		bool memberEqual = pairOne.member.compare(pairTwo.member) == 0;
		bool valueEqual = pairOne.value.compare(pairTwo.value) == 0;
		return memberEqual && valueEqual;
	}

	void setMessageWithTestValues(XPLMessage* message)
	{
		message->setMsgType("xpl-cmd");
		message->setHops(5);
		message->setDestination("vendorIdOne", "deviceIdOne", "instanceIdOne");
		message->setSource("vendorIdTwo", "deviceIdTwo", "instanceIdTwo");
		message->setSchema("schemaClass", "schemaType");
		message->addMember("nameOne", "valueOne");
		message->addMember("nameTwo", "valueTwo");
		message->addMember("nameThree", "valueThree");
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

	void assertSuccess(string functionName, bool result, string value)
	{
		if(result)
			cout << functionName +" Success!\n";
		else
			cout << functionName +" failed!\n Got: " << value << "\n";
	}
	void testGetMember(DeterminatorFactory* factory)
	{
		string member = factory->getMember("member=value");	
		bool result = (member.compare("member") == 0);
		assertSuccess("testGetMember", result, member);
	}

	void testGetValue(DeterminatorFactory* factory)
	{
		string value = factory->getValue("member=value");
		bool result = (value.compare("value") == 0);
		assertSuccess("testGetValue", result, value);
	}

	void testGetAddressParameters(DeterminatorFactory* factory)
	{
		string address = "vendorId.deviceId.instanceId";
		vector<string> parameters = factory->getAddressParameters(address);
		bool vendor = (parameters[0].compare("vendorId") == 0);
		bool device = (parameters[1].compare("deviceId") == 0);
		bool instance = (parameters[2].compare("instanceId") == 0);
		bool result = vendor && device && instance;
		string params = parameters[0] + parameters[1] + parameters[2];
		assertSuccess("testGetAddressParameters", result, params);
	}

	void testCreateMessage(DeterminatorFactory* factory)
	{
		// cout << "testCreateMessages called\n";
		string msgType = "xpl-cmd";
		int hops = 5;
		string destinationAddress = "vendorIdOne.deviceIdOne.instanceIdOne";
		string sourceAddress = "sourceVendorId.sourceDeviceId.instanceDeviceId";
		string schema = "schemaClass.schemaType";
		vector<string> parameters;
		parameters.push_back("nameOne=valueOne");
		parameters.push_back("nameTwo=valueTwo");
		parameters.push_back("nameThree=valueThree");

		// cout << "CreateXPLMessage being called...\n";
		XPLMessage* message = factory->createXPLMessage(msgType, sourceAddress, destinationAddress, schema, hops, parameters);
		// cout << "Message returned\n";
		string memberOneValue = message->findMember("nameOne");
		string memberTwoValue = message->findMember("nameTwo");
		string memberThreeValue = message->findMember("nameThree");
		bool memberResult = (memberOneValue.compare("valueOne") == 0) && (memberTwoValue.compare("valueTwo") == 0) && (memberThreeValue.compare("valueThree") == 0);

		string resultMsgType = message->getMsgType();
		XPLAddress destination = message->getDestination();
		string resultDestination = destination.vendor + "." + destination.device + "." + destination.instance; 
		XPLAddress source = message->getSource();
		string resultSource = source.vendor + "." + source.device + "." + source.instance;
		int resultHops = message->getHops();
		XPLSchema resultSchema = message->getSchema();
		string resultSchemaClass = resultSchema.schema;
		string resultSchemaType = resultSchema.type;

		bool msgTypeResult = resultMsgType.compare("xpl-cmd") == 0;
		bool destinationAddressResult = resultDestination.compare("vendorIdOne.deviceIdOne.instanceIdOne") == 0;
		bool sourceAddressResult = resultSource.compare("sourceVendorId.sourceDeviceId.instanceDeviceId") == 0;
		bool hopsResult = resultHops == hops;
		bool schemaClassResult = resultSchemaClass.compare("schemaClass") == 0;
		bool schemaTypeResult = resultSchemaType.compare("schemaType") == 0;

		bool result = msgTypeResult && destinationAddressResult && sourceAddressResult && hopsResult && schemaClassResult && schemaTypeResult;
		string resultString = resultMsgType;
		resultString.append(resultDestination);
		resultString.append(resultSource);
		resultString.append(resultSchemaClass);
		resultString.append(resultSchemaType);

		assertSuccess("testCreateMessage", result, resultString);
		// cout << "Member Result: " << memberResult << "\n";
		// cout << "Destination Address Result: " << destinationAddressResult << " " << resultDestination << "\n";
		// cout << "Source Address Result: " << sourceAddressResult << " " << resultSource << "\n";
		// cout << "Hops Result:" << hopsResult << "\n";
		// cout << "Schema Class Result: " << schemaClassResult << "\n";
		// cout << "Schema Type Result: " << schemaTypeResult << "\n";
	}

	void testCreateCondition(DeterminatorFactory* factory)
	{	
		string sourceAddress = "vendor.device.instance";
		string destinationAddress = "vendor.device.instance";
		string schema = "schemaClass.schemaType";
		string hops = "5";
		string msgType = "xpl-cmd";

		vector<string> definitions;
		definitions.push_back("nameOne=valueOne");
		definitions.push_back("nameTwo=valueTwo");
		definitions.push_back("nameThree=valueThree");
		XPLCondition* condition = factory->createXPLCondition(definitions, sourceAddress, destinationAddress, schema, hops, msgType);

		XPLMessage* message = new XPLMessage();
		message->setSource("vendor", "device", "instance");
		message->setDestination("vendor", "device", "instance");
		message->setHops(5);
		message->setSchema("schema", "type");
		message->setMsgType(msgType);
		message->addMember("nameOne", "valueOne");
		message->addMember("nameTwo", "valueTwo");
		message->addMember("nameThree", "valueThree");
		bool matched = condition->match(message);

		string resultString = ""; 

		bool result = matched;
		assertSuccess("testCreateCondition", result, resultString);
		delete condition;
	}


	void testCreateAction(DeterminatorFactory* factory)
	{
		bool result = false;
		XPLMessage* testMessage = new XPLMessage();
		setMessageWithTestValues(testMessage);
		XPLMessage* addedMessage = new XPLMessage();
		setMessageWithTestValues(addedMessage);
		vector<XPLMessage>* messages = new vector<XPLMessage>();
		messages->push_back(*testMessage);
		XPLAction* action = factory->createXPLAction(messages);

		XPLMessage executedMessage = action->execute()[0];
		result = compareXPLMessage(testMessage, &executedMessage);

		assertSuccess("testCreateAction", result, "");
	}

	void testCreateDeterminator(DeterminatorFactory* factory)
	{
		string sourceAddress = "vendorIdOne.deviceIdOne.instanceIdOne";
		string destinationAddress = "vendorIdTwo.deviceIdTwo.instanceIdTwo";
		string schema = "schema.type";
		string hops = "5";
		string msgType = "xpl-cmd";

		vector<string> definitions;
		definitions.push_back("nameOne=valueOne");
		definitions.push_back("nameTwo=valueTwo");
		definitions.push_back("nameThree=valueThree");
		XPLCondition* condition = factory->createXPLCondition(definitions, sourceAddress, destinationAddress, schema, hops, msgType);

		XPLMessage* testMessage = new XPLMessage();
		setMessageWithTestValues(testMessage);
		XPLMessage* addedMessage = new XPLMessage();
		setMessageWithTestValues(addedMessage);
		vector<XPLMessage>* messages = new vector<XPLMessage>();
		messages->push_back(*addedMessage);
		XPLAction* action = factory->createXPLAction(messages);

		Determinator* determinator = factory->createDeterminator(condition, action, true);

		string resultString = "";
		bool matched = determinator->match(testMessage);
		if(!matched)
			resultString.append("Message failed to matched. \n");

		if(!determinator->isEnabled())
			resultString.append("Determinator not enabled\n");

		XPLMessage matchedMessge = determinator->execute()[0];
		bool executedCorrectMessage = compareXPLMessage(addedMessage, &matchedMessge);
		if(!executedCorrectMessage)
			resultString.append("Failed to execute correct message.\n");

		testMessage->setMsgType("xpl-stat");
		bool notMatched = determinator->match(testMessage);
		if(!notMatched)
			resultString.append("Message matched incorrectly. \n");
			
		bool result = matched && notMatched && executedCorrectMessage && determinator->isEnabled();

		assertSuccess("testCreateDeterminator", result, resultString);
	}

	void runTests()
	{	
		DeterminatorFactory factory;
		testGetMember(&factory);
		testGetValue(&factory);
		testGetAddressParameters(&factory);
		testCreateMessage(&factory);
		testCreateCondition(&factory);
		testCreateAction(&factory);
		testCreateDeterminator(&factory);
	}
};
