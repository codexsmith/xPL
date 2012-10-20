#ifndef DayCondition_H
#define DayCondition_H

#include "XPLCommon.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include "Poco/DateTime.h"

#include "Poco/NumberFormatter.h"


#include "Poco/Logger.h"

using namespace std;



class DayCondition : public DeterminatorCondition
{


public:
    DayCondition ( pugi::xml_node );
    DayCondition();
    ~DayCondition();
    bool match ( DeterminatorEnvironment* env );
    bool equals ( DayCondition* condition );
    void appendCondition ( pugi::xml_node* inputnode );


protected:

private:
    string display_name;
    vector<bool> days;
    Logger& timeLogger;
};

#endif //DayCondition_H
