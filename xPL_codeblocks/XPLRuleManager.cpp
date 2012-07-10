#include "XPLRuleManager.h"
#include <vector>

XPLRuleManager::XPLRuleManager(vector<Determinator>* determinators)
{
    this->determinators = determinators;
}

XPLRuleManager::XPLRuleManager()
{
}

XPLRuleManager::~XPLRuleManager()
{
}

vector<XPLMessage> XPLRuleManager::match(XPLMessage msg)
{
    vector<XPLMessage> messagesToSend, messagesFromDeterminator;

    //match stuff
    for (int i = 0; i < determinators->size(); i++)
    {
        if (determinators->at(i).match(&msg))
        {
            messagesFromDeterminator = determinators->at(i).execute();
            messagesToSend.insert(messagesToSend.end(), messagesFromDeterminator.begin(), messagesFromDeterminator.end());
        }
    }

    //return list of response messages from matches'
    return messagesToSend;
}
