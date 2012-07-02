// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>

using namespace std;

class XPLAddress
{
    public:
    string vendor, device, instance;
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

#endif // XPLHal_H
