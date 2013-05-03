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
#include "XplMsg.h"

using namespace std;
using namespace Poco;
using namespace xpl;
/*
class XplMsg;*/

class DeterminatorEnvironment
{
public:

    enum determinatorEventType { xPLMessage, globalChanged, none};

    DeterminatorEnvironment();
    DeterminatorEnvironment ( AutoPtr<XplMsg> );
    DeterminatorEnvironment ( string globalname );
    //~XPLRuleManager();

    string replaceSingleValue ( const string input ) ;
    string handleValueReplacement ( const string input );


    // we keep out own copy so that any GlobalActions don't confuse any GlobalConditions //not anymore
    //GlobalManager globals;
    SharedPtr<GlobalManager> globals;
    //populated if a global changed
    string globalName;
    //populated if a message came in
    AutoPtr<XplMsg> message;
    Timestamp mtime;
    determinatorEventType envType;

};

#endif //DeterminatorEnvironment_h
