#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <sstream>

class XHCPDispatcher{
public:

    XHCPDispatcher(std::string);
    XHCPDispatcher();
    std::string addRule(std::string );
    std::string listRules(std::string );
    std::string deleteRule(std::string );
    std::string capCommand(std::string );
private:
    std::string name;
};

#endif
