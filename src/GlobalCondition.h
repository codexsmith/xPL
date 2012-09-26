#ifndef GlobalCondition_H
#define GlobalCondition_H

#include "GlobalManager.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"


using namespace std;

class GlobalCondition : public DeterminatorCondition {
public:
    GlobalCondition( string nameIn, string operatorIn, string valueIn );
    GlobalCondition(pugi::xml_node);
    bool match(DeterminatorEnvironment* env);
    bool equals(GlobalCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
protected:

private:
    string display_name;
    string globalName;
    DeterminatorOperator globalOperator;
    string globalValue;
};

#endif //GlobalCondition_H
