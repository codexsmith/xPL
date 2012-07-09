#include <vector>

#include "XPLAction.h"
#include "XPLMessage.h"

using namespace std;

XPLAction::XPLAction(vector<XPLMessage>* responses)
{
	responses_ = responses;
}

XPLAction::~XPLAction()
{
	delete responses_;
}

vector<XPLMessage> XPLAction::execute()
{
	return *responses_;
}

bool XPLAction::equals(XPLAction* action)
{
	for(int i = 0; i < responses_->size(); i++)
	{
			
	}
}
