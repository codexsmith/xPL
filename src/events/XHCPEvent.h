#ifndef XHCPEvent_H
#define XHCPEvent_H

#include "../XPLCommon.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include "Poco/Timestamp.h"
#include <Poco/DateTime.h>


using namespace std;
using namespace Poco;

class XHCPEvent
{
public:
    XHCPEvent ( string description );
    
    string getDescription();
    string getFullText();
    Poco::Timestamp getNextTimeOrNull();
    
    bool daysofweek_[7];
    string subname_;
    string params_;
    int randomizeMins_;
    int startTimeMins_;
    int endTimeMins_;
    int interval_;
    DateTime date_;
    string tag_;
};

#endif //XHCPEvent_H