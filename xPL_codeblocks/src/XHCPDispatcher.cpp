#include "XHCPDispatcher.h"
#include "XPLRuleManager.h"



    XHCPDispatcher::XHCPDispatcher(std::string aName){
        name = aName;
    }
    XHCPDispatcher::XHCPDispatcher(){

    }
    std::string XHCPDispatcher::addRule(std::string aString){
        std::string theString = "";
        theString += "a \"";
        theString += aString;
        theString += "\"\r\n";
        return aString;
    }
    std::string XHCPDispatcher::listRules(std::string aString){
        std::string theString;
        XPLRuleManager* ruleMgr = XPLRuleManager::Instance();
        
        theString +=  "237 List of Determinator Rules follows\r\n";
        
        //TODO deal with the thread-saftey issue here
        //TODO handle args like a group name or "{ALL}"
        vector<Determinator*>* dets = ruleMgr->determinators;
        for(vector<Determinator*>::iterator dit = dets->begin(); dit!=dets->end(); ++dit) {
            string detString;
            detString += (*dit)->getGUID() + "\t";
            detString += (*dit)->name + "\t";
            detString += (*dit)->isEnabled() + "\t";
            theString += detString + "\r\n";
        }
        theString += ".\r\n";
        //String += "b\r\n";
        return theString;
    }
    std::string XHCPDispatcher::deleteRule(std::string aString){
        return "wee\r\n";
    }
    std::string XHCPDispatcher::capCommand(std::string aString){
        std::string theString = "236 --010L0\r\n";
        return theString;
    }

