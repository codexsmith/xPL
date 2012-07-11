#include "DeterminatorFactory.h"
#include <string>

void testGetMember(DeterminatorFactory* factory)
{
	string member = factory->getMember("member=value");	
	if(member.compare("member") == 0)
		cout << "testGetMember Success!\n";
	else
		cout << "testGetMember failed!\n Got: " << member <<"\nExpected: member";
}

void testGetValue(DeterminatorFactory* factory)
{
	string value = factory->getValue("member=value");
	if(value.compare("value") == 0)
		cout << "testGetValue Success!\n";
	else
		cout << "testGetValue failed!\n Got: " << value << "\n";
}

int main()
{	
	DeterminatorFactory factory;
	testGetMember(&factory);
	testGetValue(&factory);

	return 0;
}
