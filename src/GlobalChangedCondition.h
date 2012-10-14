#ifndef GlobalChangedCondition_H
#define GlobalChangedCondition_H

#include "GlobalManager.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"


using namespace std;

class GlobalChangedCondition : public DeterminatorCondition {
public:
    GlobalChangedCondition( string nameIn);
    GlobalChangedCondition(pugi::xml_node);
    bool match(DeterminatorEnvironment* env);
    bool equals(GlobalChangedCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
protected:

private:
    string display_name;
    string globalName;
};

#endif //GlobalChangedCondition_H
