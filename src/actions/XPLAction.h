#ifndef XPLAction_H
#define XPLAction_H

#include <vector>
#include "DeterminatorAction.h"
#include "pugixml/pugixml.hpp"
#include "xplMsg.h"
#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"
//#include "DeterminatorEnvironment.h"

class DeterminatorEnvironment;

using namespace std;

class XPLAction : public DeterminatorAction
{
public:
    XPLAction();
    XPLAction ( pugi::xml_node );
    virtual ~XPLAction() {};
    void execute ( DeterminatorEnvironment* env );
    void appendAction ( pugi::xml_node* outputnode );
    bool equals ( XPLAction* action );
    //XPLMessage message;
    AutoPtr<xplMsg> message;
    Logger& actlog;

};

#endif //XPLAction_H
