#ifndef XPLAction_H
#define XPLAction_H

#include <vector>
#include "XPLMessage.h"
#include "DeterminatorAction.h"
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"
using namespace std;

class XPLAction : public DeterminatorAction {
	public:
		XPLAction(vector<XPLMessage>* responses);
    XPLAction(pugi::xml_node);
		~XPLAction();
    void execute(DeterminatorEnvironment* env);
    void appendAction(pugi::xml_node* outputnode);
		bool equals(XPLAction* action);
    XPLMessage message;

	protected:
		vector<XPLMessage>* getResponses();

	private:
		vector<XPLMessage>* responses_;

    
};

#endif //XPLAction_H
