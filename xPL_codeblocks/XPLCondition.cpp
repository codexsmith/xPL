#include "XPLCondition.h"

using namespace std;

XPLCondition::XPLCondition(vector<XPLValuePair>* attributes)
{
	attributes_ = attributes;
}

XPLCondition::~XPLCondition()
{
}

bool XPLCondition::match(XPLMessage* message)
{
}
