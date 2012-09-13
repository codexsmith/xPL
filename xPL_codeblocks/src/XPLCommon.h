
#ifndef XPLCommon_H
#define XPLCommon_H

#include <string>
#include <vector>

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

//extern xPL_ServicePtr theService;

#endif