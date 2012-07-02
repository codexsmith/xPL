#include "Determinator.h"
#include "XPLCondition.h"
#include "XPLAction.h"

Determinator::Determinator(XPLCondition condition, XPLAction action)
{
	XPLCondition condition_ = condition;
	XPLAction action_ = action;
	enabled_ = true;
}

Determinator::Determinator(Determinator* determinator)
{
	condition_ = determinator->getCondition();
	action_ = determinator->getAction();
}

Determinator::~Determinator()
{
}

XPLAction Determinator::getAction()
{
	return action_;
}

XPLCondition Determinator::getCondition()
{
	return condition_;
}

bool Determinator::match(XPLMessage message)
{
	return condition_.match(message);
}

vector<XPLMessage> Determinator::execute()
{
	action_.execute();
}

void Determinator::setEnabled(bool enable)
{
	enabled_ = enable;
}

bool Determinator::isEnabled()
{
	return enabled_;
}
