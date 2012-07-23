#include "DeterminatorFactory.h"
#include "XPLHal.h"
#include "Determinator.h"
#include "DeterminatorFactory.h"
#include "XPLCondition.h"
#include "XPLMessage.h"
#include "XPLAction.h"

#include <iostream>
#include <string>


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
	bool result = false;
	vector<string> definitions;
	definitions.push_back("nameOne=valueOne");
	definitions.push_back("nameTwo=valueTwo");
	definitions.push_back("nameThree=valueThree");
	XPLCondition* condition = factory->createCondition(definitions);

	XPLMessage* message = new XPLMessage();
	condition->match(message);

	assertSuccess("testCreateMessage", result, "");
}


void testCreateAction(DeterminatorFactory* factory)
{
	bool result = false;
	assertSuccess("testCreateAction", result, "");
}

void testCreateDeterminator(DeterminatorFactory* factory)
{
	bool result = false;
	assertSuccess("testCreateDeterminator", result, "");
}

int main()
{	
	DeterminatorFactory factory;
	testGetMember(&factory);
	testGetValue(&factory);
	testGetAddressParameters(&factory);
	//cout << "testGetAddressParameters returned\n";
	testCreateMessage(&factory);
	testCreateCondition(&factory);
	testCreateAction(&factory);
	testCreateDeterminator(&factory);

	return 0;
}
