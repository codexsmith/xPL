#include "XPLHal.h"


#include <vector>
#include <string>
#include <iostream>

#include "XPLAction.h"
#include "XPLMessage.h"
#include "XPLParser.h"
#include "DeterminatorFactory.h"



using namespace std;

XPLHal::XPLHal() {
    cout << "XPLHal Created\n";
    //Load XML Determinators from disk.
    Poco::SharedPtr<XPLRuleManager> ruleMgr = XPLRuleManager::Instance();
    
}
XPLHal::~XPLHal() {
    cout << "XPLHal Destroying\n";
    saveDeterminators();
    cout << "XPLHal Destroyed\n";
}


void XPLHal::saveDeterminators(void) {
    cout << "trying to save determinators\n";
    ruleMgr->saveDeterminators();
}