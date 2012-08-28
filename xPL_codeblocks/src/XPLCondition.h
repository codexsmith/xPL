#ifndef XPLCondition_H
#define XPLCondition_H

#include "XPLHal.h"
#include "XPLMessage.h"
#include <string>
#include "pugixml/pugixml.hpp"


using namespace std;
static const string tabs = "\t\t";

class XPLCondition {
	public:
		explicit XPLCondition(vector<XPLValuePair>* attributes, XPLAddress sourceAddress, XPLAddress destinationAddress, XPLSchema schema, int hops, string msgType);
		~XPLCondition();
		bool match(XPLMessage* message);
		bool equals(XPLCondition* condition);
    void appendCondition( pugi::xml_node* inputnode );
		string printXML();

	protected:
		vector<XPLValuePair>* getAttributes();

	private:
		vector<XPLValuePair>* attributes_;
		XPLAddress sourceAddress_;
		XPLAddress destinationAddress_;
		XPLSchema schema_;
		string msgType_;
		int hops_;


};

#endif //XPLAction_H
