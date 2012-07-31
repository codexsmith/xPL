#include "Determinator.h"
#include "XPLCondition.h"
#include "XPLAction.h"

#include <vector>
#include <iostream>

Determinator::Determinator()
{
}

Determinator::Determinator(XPLCondition* condition, XPLAction* action)
{
	condition_ = condition;
	action_ = action;
	// cout << "Setting enabled to true!\n";
	enabled_ = true;
	// cout << enabled_ << "\n";
}

//Determinator::Determinator(const Determinator& determinator)
//{
//	condition_ = determinator.getCondition();
//	action_ = determinator.getAction();
//	cout << "The copy constructor got called!";
//}

Determinator::~Determinator()
{
}

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

bool Determinator::match(XPLMessage* message)
{
	if(enabled_)
	{
		// cout << "Determinator is enabled and called match\n";
		return condition_->match(message);
	}
	// cout << "Determinator is not enabled but did call match\n";
	return false;
}

vector<XPLMessage> Determinator::execute()
{
	return action_->execute();
}

void Determinator::setEnabled(bool enable)
{
	enabled_ = enable;
}

bool Determinator::isEnabled()
{
	return enabled_;
}
