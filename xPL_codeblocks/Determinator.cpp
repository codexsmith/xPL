#include "Determinator.h"
#include "XPLCondition.h"
#include "XPLAction.h"

#include <vector>
#include <iostream>

Determinator::Determinator(XPLCondition* condition, XPLAction* action)
{
	condition_ = condition;
	action_ = action;
	enabled_ = true;
}

Determinator::~Determinator()
{
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
		return condition_->match(message);
	}
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
