// xPL Linux Hal Server

#ifndef XPLHal_H
#define XPLHal_H

#include <iostream>
#include <stdio.h>
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
