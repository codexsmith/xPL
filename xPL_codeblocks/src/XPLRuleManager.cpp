#include "XPLRuleManager.h"
#include <vector>
#include <iostream>


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
std::string XPLRuleManager::detToString(){
    std::string theString;
    for (int i = 0; i < determinators->size();i++){
        //string += determinators -> at(i).getID();
        //string += "\r\n";
    }
    return theString;
}
vector<XPLMessage> XPLRuleManager::match(XPLMessage msg)
{
    vector<XPLMessage> messagesToSend, messagesFromDeterminator;

    //match stuff
    for (int i = 0; i < determinators->size(); i++)
    {
        cout<<"checking" + i;
        if (determinators->at(i).match(&msg))
        {
            cout<<"matched";
            //messagesFromDeterminator = determinators->at(i).execute();
            determinators->at(i).execute();
            //messagesToSend.insert(messagesToSend.end(), messagesFromDeterminator.begin(), messagesFromDeterminator.end());
        }
    }

    //return list of response messages from matches'
    return messagesToSend;
}
