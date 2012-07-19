#include "DeterminatorFactory.h"
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
	string destinationAddress;
	bool result = false;
	assertSuccess("testCreateMessage", result, "");
}

void testCreateCondition(DeterminatorFactory* factory)
{
	
	bool result = false;
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
	testCreateMessage(&factory);
	testCreateCondition(&factory);
	testCreateAction(&factory);
	testCreateDeterminator(&factory);

	return 0;
}
