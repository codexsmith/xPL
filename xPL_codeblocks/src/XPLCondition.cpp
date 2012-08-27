#include "XPLCondition.h"
#include "XPLHal.h"
#include "XPLMessage.h"

#include <string>
#include <iostream>

using namespace std;

//XPLConditions should only be constructed in an appropriate factory. Improper values may cause
//matching issues. All XPLMessage header parameters are stored in their own fields, while body
//parameters are passed in via a vector of XPLValuePairs.
XPLCondition::XPLCondition(vector<XPLValuePair>* attributes, XPLAddress sourceAddress, XPLAddress destinationAddress, XPLSchema schema, int hops, string msgType)
{
	msgType_ = msgType;
	sourceAddress_ = sourceAddress;
	destinationAddress_ = destinationAddress;
	schema_ = schema;
	hops_ = hops;
	attributes_ = attributes;
}

XPLCondition::~XPLCondition()
{
	for(int i = 0; i < attributes_->size(); i++)
	{
		attributes_->pop_back();
	}
	delete attributes_;
}


//Matches a message. Returns true when all of the CONDITION'S parameters have
//matching values in the passed in message.
bool XPLCondition::match(XPLMessage* message)
{
	XPLAddress sourceAddress = message->getSource();
	XPLAddress destinationAddress = message->getDestination();
	int hops = message->getHops();

//	bool msgMatch = (msgType_.compare(message->getMsgType()) == 0);
//	bool hopsMatch = (hops_ == hops) || hops_ == NULL;
//	bool sourceMatch = (sourceAddress_.vendor.compare(sourceAddress.vendor) == 0) && (sourceAddress_.device.compare(sourceAddress.device) == 0) && (sourceAddress_.instance.compare(sourceAddress.instance) == 0) || sourceAddress == NULL;
//	bool destinationMatch = (destinationAddress_.vendor.compare(destinationAddress.vendor) == 0) && (destinationAddress_.device.compare(destinationAddress.device) == 0) && (destinationAddress_.instance.compare(destinationAddress.instance) == 0) || destinationAddress == NULL;

	bool membersMatch = true;
	for(int i = 0; i < attributes_->size(); i++)
	{
		XPLValuePair memberToFind = attributes_->at(i);
		string value = message->findMember(memberToFind.member);
		if(!(memberToFind.value.compare(value) == 0))
		{
			membersMatch = false;
			break;
		}
	}
	return membersMatch;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool XPLCondition::equals(XPLCondition* condition)
{
	vector<XPLValuePair>* compareTo = condition->getAttributes();
	if(compareTo->size() != attributes_->size())
		return false;
	for(int i = 0; i < attributes_->size(); i++)
	{
		XPLValuePair* attribute = &attributes_->at(i);
		XPLValuePair* compare = &compareTo->at(i);
		if(attribute->member.compare(compare->member) != 0)
			return false;
		if(attribute->value.compare(compare->value) != 0)
			return false;
	}
	return true;
}

vector<XPLValuePair>* XPLCondition::getAttributes()
{
	return attributes_;
}

//Turns the XPLCondition into a formatted XML string
//for serialization.
string XPLCondition::printXML()
{
	string result = "";
	result.append("\t<xplcondition");
	result.append("\n\tmsg_type=");
	result.append(msgType_);
	result.append("\n\tsource_vendor=");
	result.append(sourceAddress_.vendor);
	result.append("\n\tsource_device=");
	result.append(sourceAddress_.device);
	result.append("\n\tsource_instance=");
	result.append(sourceAddress_.instance);
	result.append("\n\ttarget_vendor=");
	result.append(destinationAddress_.vendor);
	result.append("\n\ttarget_device=");
	result.append(destinationAddress_.device);
	result.append("\n\ttarget_instance=");
	result.append(destinationAddress_.instance);
	result.append("\n\tschema_class=");
	result.append(schema_.schema);
	result.append("\n\tschema_type=");
	result.append(schema_.type);
	result.append("\n\t>\n");

	for(int i = 0; i<attributes_->size(); i++)
	{
		result.append("\t\t<param name=" + attributes_->at(i).member + "\n");
		result.append("\t\toperator=\n");
		result.append(tabs + "value=" + attributes_->at(i).value + "\n");
		result.append(tabs + ">\n");
	}
	result.append("</xplcondition>\n");
	return result;
}
