#ifndef XPLCondition_H
#define XPLCondition_H

#include "XPLCommon.h"
#include "XPLMessage.h"
#include "DeterminatorCondition.h"
#include "DeterminatorEnvironment.h"
#include <string>
#include "pugixml/pugixml.hpp"


using namespace std;
static const string tabs = "\t\t";

class XPLCondition : public DeterminatorCondition {
public:
    XPLCondition( std::vector< XPLValuePair>* attributes, XPLAddress sourceAddress, XPLAddress destinationAddress, XPLSchema schema, int hops, string msgType );
    XPLCondition(pugi::xml_node);
    XPLCondition();
    bool match(DeterminatorEnvironment* env);
    bool equals(XPLCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
    string printXML();

protected:
    vector<XPLValuePair>* getAttributes();

private:
    string display_name;
    vector<XPLValuePair> attributes_;
    XPLAddress sourceAddress_;
    XPLAddress destinationAddress_;
    XPLSchema schema_;
    string msgType_;
    int hops_;


};

#endif //XPLAction_H
