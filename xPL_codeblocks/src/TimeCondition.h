#ifndef TimeCondition_H
#define TimeCondition_H

#include "XPLHal.h"
#include "XPLMessage.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"


using namespace std;

class TimeCondition : public DeterminatorCondition {
public:
    TimeCondition(pugi::xml_node);
    TimeCondition();
    ~TimeCondition();
    bool match(DeterminatorEnvironment* env );
    bool equals(TimeCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
    
protected:
    
private:
    string display_name;
    string toprator;
    int thours;
    int tminutes;

};

#endif //TimeCondition_H
