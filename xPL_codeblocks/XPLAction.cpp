#include <vector>

#include "XPLAction.h"
#include "XPLMessage.h"

using namespace std;

XPLAction::XPLAction()
{
}

XPLAction::XPLAction(vector<XPLMessage>* responses)
{
	responses_ = responses;
}

XPLAction::~XPLAction()
{
}

vector<XPLMessage> XPLAction::execute()
{
	//return responses_;
}

