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

void XPLRuleManager::match(XPLMessage msg)
{

    //match stuff
    for (int i = 0; i < determinators->size(); i++)
    {
        if (determinators->at(i).match(&msg))
        {
            determinators->at(i).execute();
        }
    }
}
