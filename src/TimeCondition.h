#ifndef TimeCondition_H
#define TimeCondition_H

#include "XPLCommon.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include "Poco/DateTime.h"

#include "Poco/NumberFormatter.h"


#include "Poco/Logger.h"

using namespace std;



class TimeCondition : public DeterminatorCondition {
    
    
public:
    TimeCondition(pugi::xml_node);
    TimeCondition();
    ~TimeCondition();
    bool match(DeterminatorEnvironment* env );
    bool equals(TimeCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
    
    enum categoryValue
    {
        cat_time, cat_date, cat_day, cat_month, cat_year
    };
    
    typedef std::map<std::string, TimeCondition::categoryValue> categoryMapT;
    typedef std::map<TimeCondition::categoryValue, std::string> categoryMapTr;
    typedef std::map<TimeCondition::categoryValue, std::string> categoryFormatMapT;
    
    
    
protected:
    
private:
    string display_name;
    string toprator;
    int thours;
    int tminutes;
    categoryValue tcategory;
    DateTime tValue;
    Logger& timeLogger;
};

#endif //TimeCondition_H
