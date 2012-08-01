#include "DeterminatorFactory.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// string splitString(string* stringToSplit, char delimiter)
// {
// 	int splitIndex = stringToSplit->find_first_of(delimiter);
// 	string retVal = *(stringToSplit->substr(0, splitIndex));
// 	stringToSplit = stringToSplit->substr(splitIndex+1);
// 	return retVal;
// }

DeterminatorFactory::DeterminatorFactory()
{
}

DeterminatorFactory::~DeterminatorFactory()
{
}

Determinator* DeterminatorFactory::createDeterminator(vector<string> determinatorDefintion, vector<string> conditionDefinition, vector<string> actionDefintion)
{
	Determinator* determinator;
	XPLCondition* condition;
	XPLAction* action;

	return determinator;
}

Determinator* DeterminatorFactory::createDeterminator(XPLCondition* condition, XPLAction* action, bool enabled)
{
	Determinator* determinator = new Determinator(condition, action);
	determinator->setEnabled(enabled);
	return determinator;
}

XPLCondition* DeterminatorFactory::createXPLCondition(vector<string> conditions, string source, string destination, string schemaParam, string hopsParam, string msgType)
{

	XPLAddress sourceAddress;
	vector<string> parameters = getAddressParameters(source);
	sourceAddress.vendor = parameters[0];
	sourceAddress.device = parameters[1];
	sourceAddress.instance = parameters[2];

	XPLAddress destinationAddress; 
	parameters = getAddressParameters(destination);
	destinationAddress.vendor = parameters[0];
	destinationAddress.device = parameters[1];
	destinationAddress.instance = parameters[2];

	XPLSchema schema;
	schema.schema = getSchemaClass(schemaParam);
	schema.type = getSchemaType(schemaParam);

	int hops = atoi(hopsParam.c_str());

	vector<XPLValuePair>* conditionVector = new vector<XPLValuePair>();
	for(int i = 0; i<conditions.size(); i++)
	{
		XPLValuePair* valuePair = new XPLValuePair();
		valuePair->member = getMember(conditions.at(i));
		valuePair->value = getValue(conditions.at(i));
		conditionVector->push_back(*valuePair);
	}
	XPLCondition* condition = new XPLCondition(conditionVector, sourceAddress, destinationAddress, schema, hops, msgType);
	return condition;
}

XPLAction* DeterminatorFactory::createXPLAction(vector<XPLMessage>* messages)
{
	vector<XPLMessage>* actionVector = new vector<XPLMessage>();
	for(int i = 0; i<messages->size(); i++)
	{
		actionVector->push_back(messages->at(i));
	}
	XPLAction* action = new XPLAction(actionVector);
	return action;
}

//TODO: Error checking for individuals fields
XPLMessage* DeterminatorFactory::createXPLMessage(string msgType, string sourceAddress, string destinationAddress, string schema, int hops, vector<string>* parameters)
{
	XPLMessage* message = new XPLMessage();
	message->setMsgType(msgType);
	vector<string> addressParameters = getAddressParameters(sourceAddress);
	message->setSource(addressParameters[0], addressParameters[1], addressParameters[2]);
	addressParameters = getAddressParameters(destinationAddress);
	message->setDestination(addressParameters[0], addressParameters[1], addressParameters[2]);
	int schemaSplit = schema.find(".");
	string schemaClass = schema.substr(0, schemaSplit);
	string schemaType = schema.substr(schemaSplit+1, schema.length() - schemaSplit);
	message->setSchema(schemaClass, schemaType);
	message->setHops(hops);
	for(int i = 0; i<(*parameters).size(); i++)
	{
		message->addMember(getMember((*parameters)[i]), getValue((*parameters)[i]));
	}
	return message;
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

vector<string> DeterminatorFactory::getAddressParameters(string address)
{
	vector<string> parameters;
	int position;
	while(parameters.size()<3)
	{
		position = address.find_first_of(".");
		parameters.push_back(address.substr(0,position));
		address = address.substr(position+1, address.length());
	}
	return parameters;
}

string DeterminatorFactory::getSchemaClass(string definition)
{
	int splitIndex = definition.find_first_of(".");
	definition = definition.substr(0, splitIndex);
	return definition;
}

string DeterminatorFactory::getSchemaType(string definition)
{
	int splitIndex = definition.find_first_of(".");
	definition = definition.substr(splitIndex+1, definition.length());
	return definition;	
}
