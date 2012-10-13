#ifndef DeterminatorEnvironment_H
#define DeterminatorEnvironment_H

#include "XPLCommon.h"
#include "GlobalManager.h"
#include "Poco/AutoPtr.h"
#include "Poco/Timestamp.h"
//#include "XPLMessage.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include <ctime>
#include "xplMsg.h"

using namespace std;
using namespace Poco;
using namespace xpl;
/*
class xplMsg;*/

class DeterminatorEnvironment
{
public:
    DeterminatorEnvironment();
    DeterminatorEnvironment ( xplMsg*  );
    DeterminatorEnvironment ( string globalname  );
    //~XPLRuleManager();
    // we keep out own copy so that any GlobalActions don't confuse any GlobalConditions
    //GlobalManager globals;
    SharedPtr<GlobalManager> globals;
    //populated if a global changed
    string globalName;
    //populated if a message came in
    xplMsg* message;
    Timestamp mtime;

};

#endif //DeterminatorEnvironment_h
