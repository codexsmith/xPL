#ifndef DeterminatorEnvironment_H
#define DeterminatorEnvironment_H

#include "XPLCommon.h"
#include "Poco/AutoPtr.h"
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

    //~XPLRuleManager();

    xplMsg* message;
    struct tm * mtime;

};

#endif //DeterminatorEnvironment_h
