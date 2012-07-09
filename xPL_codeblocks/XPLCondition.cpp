#include "XPLCondition.h"
#include "XPLHal.h"

using namespace std;

XPLCondition::XPLCondition(vector<XPLValuePair>* attributes)
{
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
