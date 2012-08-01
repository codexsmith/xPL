#include "XPLCondition.h"
#include "XPLHal.h"
#include "XPLMessage.h"

#include <string>
#include <iostream>

using namespace std;

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

bool XPLCondition::match(XPLMessage* message)
{
	XPLAddress sourceAddress = message->getSource();
	XPLAddress destinationAddress = message->getDestination();
	int hops = message->getHops();

	bool msgMatch = (msgType_.compare(message->getMsgType()) == 0) || (msgType_ == NULL);
	bool hopsMatch = (hops_ == hops) || hops_ == NULL;
	bool sourceMatch = (sourceAddress_.vendor.compare(sourceAddress.vendor) == 0) && (sourceAddress_.device.compare(sourceAddress.device) == 0) && (sourceAddress_.instance.compare(sourceAddress.instance) == 0) || sourceAddress == NULL;
	bool destinationMatch = (destinationAddress_.vendor.compare(destinationAddress.vendor) == 0) && (destinationAddress_.device.compare(destinationAddress.device) == 0) && (destinationAddress_.instance.compare(destinationAddress.instance) == 0) || destinationAddress == NULL;
	
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
	return msgMatch && membersMatch && msgMatch && sourceMatch && destinationMatch && hopsMatch;
}

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
