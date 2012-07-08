#include "XPLRuleManager.h"
#include <vector>

XPLRuleManager::XPLRuleManager(vector<Determinator> determinators)
{
    this->determinators = determinators;
}

XPLRuleManager::XPLRuleManager()
{
}

XPLRuleManager::~XPLRuleManager()
{
}

vector<XPLMessage> XPLRuleManager::match(XPLMessage)
{
    //match stuff

    //return list of response messages from matches
}
