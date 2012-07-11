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

void testCreateCondition(DeterminatorFactory* factory)
{
	string definitions[] = {"-determinator", "-condition", "memberOne=valueOne", "memberTwo=valueTwo"};
	XPLCondition* condition = factory->createCondition(definitions, 2);
	cout << "Made it out of createCondition!\n";
	XPLMessage message;
	message.addMember("memberOne","valueOne");
	message.addMember("memberTwo","valueTwo");
	if(condition->match(&message))
		cout << "testCreateCondition Success!\n";
	else
		cout << "testCreateCondition failed!\n";
}

void testCreateAction(DeterminatorFactory* factory)
{
	string definitions[] = {"-action"};
	factory->createAction(definitions, 0);
}

int main()
{	
	DeterminatorFactory factory;
	testGetMember(&factory);
	testGetValue(&factory);
	testCreateCondition(&factory);

	return 0;
}
