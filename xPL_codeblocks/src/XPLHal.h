// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>

extern "C" {

    #include "xPLLib/xPL.h"

}

using namespace std;

class XPLAddress
{
    public:
    string vendor, device, instance;
    XPLAddress(){
            vendor="*";
            device="*";
            instance="*";
    }
};

class XPLSchema
{
    public:
    string schema, type;
};

class XPLValuePair
{
    public:
    string member, value;
};

extern xPL_ServicePtr theService;

#endif // XPLHal_H
