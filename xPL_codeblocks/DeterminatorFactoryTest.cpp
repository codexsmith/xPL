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
	
}

void testCreateMessage(DeterminatorFactory* factory)
{
	string destinationAddress;
}

void testCreateCondition(DeterminatorFactory* factory)
{
	
}


void testCreateAction(DeterminatorFactory* factory)
{

}

int main()
{	
	DeterminatorFactory factory;
	testGetMember(&factory);
	testGetValue(&factory);
	testCreateCondition(&factory);

	return 0;
}
