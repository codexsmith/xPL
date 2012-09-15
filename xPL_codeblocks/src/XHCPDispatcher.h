#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Poco/SharedPtr.h"
#include <sstream>

using namespace Poco;
class XPLRuleManager;

class XHCPDispatcher{
public:

    XHCPDispatcher(std::string);
    XHCPDispatcher( SharedPtr<XPLRuleManager> );
    std::string addRule(std::string );
    std::string listRules(std::string );
    std::string deleteRule(std::string );
    std::string capCommand(std::string );
private:
    std::string name;
    SharedPtr< XPLRuleManager > ruleMgr;
};

#endif
