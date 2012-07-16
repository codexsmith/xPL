#include "DeterminatorFactory.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"
#include <string>
#include <vector>

DeterminatorFactory::DeterminatorFactory()
{
}

DeterminatorFactory::~DeterminatorFactory()
{
}

Determinator* DeterminatorFactory::createDeterminator(string definitions[])
{
	Determinator* determinator;	
	if(definitions[0].compare("-determinator") == 0)
	{	
		XPLCondition* condition;
		XPLAction* action;
	}
	return determinator;
}

XPLCondition* DeterminatorFactory::createCondition(string definitions[])
{
	vector<XPLValuePair>* conditionVector = new vector<XPLValuePair>();
	for(int i = 0; i<definitions->length; i++)
	{
		XPLValuePair* valuePair = new XPLValuePair(); 
		valuePair->member = getMember(definitions[index]);
		valuePair->value = getValue(definitions[index]);
		conditionVector->push_back(*valuePair);
	}
	XPLCondition* condition = new XPLCondition(conditionVector);
	return condition;
}

XPLAction* DeterminatorFactory::createAction(xPLMessage messages[])
{
	vector<XPLMessage>* actionVector = new vector<XPLMessage>();
	for(int i = 0; i<messages.length; i++)
	{
		actionVector.push_back(messages[i]);
	}
	XPLAction* action = new XPLAction(actionVector);
	return action;	
}

//TODO: Error checking for individuals fields
XPLMessage* DeterminatorFactory::createsXPLMessage(string msgType, string sourceAddress, string destinationAddress, int hops, string parameters[])
{
	XPLMessage* message = new XPLMessage();
	message->setMsgType(msgType);
	string addressParameters[] = getAddressParameters(sourceAddress);
	message->setSource(addressParameters[0], addressParameters[1], addressParameters[2]);
	addressParameters = getAddressParameters(destinationAddress);
	message->setSource(addressParameters[0], addressParameters[1], addressParameters[2]);
	
}

string DeterminatorFactory::getMember(string definition)
{
	int splitIndex = definition.find_first_of("=");
	definition = definition.substr(0, splitIndex);
	return definition;
}

string DeterminatorFactory::getValue(string definition)
{
	int splitIndex = definition.find_first_of("=");
	definition = definition.substr(splitIndex+1, definition.length());
	return definition;	
}

string* DeterminatorFactory::getAddressParameters(string address)
{
	string parameters[3];
	int paramIndex = 0;
	for(int i = 0; i<address.length; i++)
	{
		if(address.at(i) == ".")
		{
			parameters[0] = address.substr(0, i);
			paramIndex++;
			address = address.substr(i+1, address.length);	
		}	
	}
	return parameters;
}
