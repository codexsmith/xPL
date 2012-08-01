#include "Determinator.h"
#include "XPLCondition.h"
#include "XPLAction.h"

#include <vector>
#include <iostream>

//default constructor
Determinator::Determinator()
{
}

//Determinators should be constructed by the DeterminatorFactory only.
//User must have already created the appropriate condition and action.
//New Determinators are always enabled.
Determinator::Determinator(XPLCondition* condition, XPLAction* action)
{
	condition_ = condition;
	action_ = action;
	enabled_ = true;
}

Determinator::~Determinator()
{
}

//Used for serialization in of determinators
void Determinator::setAction(XPLAction* action)
{
    action_ = action;
}

XPLAction* Determinator::getAction()
{
	return action_;
}

XPLCondition* Determinator::getCondition()
{
	return condition_;
}

string Determinator::getGUID()
{
	return GUID_;
}

void Determinator::setGUID(string GUID)
{
	GUID_ = GUID;
}

//Passthrough to XPLCondition to test if the message matches. 
//Always fails when the Determinator is disabled.
bool Determinator::match(XPLMessage* message)
{
	if(enabled_)
	{
		return condition_->match(message);
	}
	return false;
}

//pass through to XPLAction. When a determinator is matched, it executes its action.
//Future releases should add other action executions here.
vector<XPLMessage> Determinator::execute()
{
	return action_->execute();
}

//Disabled determinators can never be executed. They are on by default.
void Determinator::setEnabled(bool enable)
{
	enabled_ = enable;
}

//Tests if this determinator can be executed or matched.
bool Determinator::isEnabled()
{
	return enabled_;
}

//Turns Determinator and its collaborators into formatted XML
//for serialization.
string Determinator::printXML()
{
	string result = "";
	result.append("<determinator ");
	result.append("guid=");
	result.append(GUID_);
	string enabled = "N";
	if(enabled_)
		enabled = "Y";
	result.append(" enabled=");
	result.append(enabled);
	result.append(">");
	result.append("\n\t<input match=\"any\">\n");
	string conditionXML = condition_->printXML();
	result.append(conditionXML);
	result.append("\n\t</input>\n");
	result.append("\n\t<output>\n");
	string actionXML = action_->printXML();
	result.append(actionXML);
	result.append("\n\t</output>\n");
	result.append("</determinator>");

	return result;
}
