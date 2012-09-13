#ifndef DeterminatorEnvironment_H
#define DeterminatorEnvironment_H

#include "XPLCommon.h"
//#include "XPLMessage.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include <ctime>

using namespace std;

class XPLMessage;

class DeterminatorEnvironment
{
public:
    DeterminatorEnvironment();
    DeterminatorEnvironment ( XPLMessage* );

    //~XPLRuleManager();

    XPLMessage* message;
    struct tm * mtime;

};

#endif //DeterminatorEnvironment_h
