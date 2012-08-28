#ifndef XPLAction_H
#define XPLAction_H

#include <vector>
#include "XPLMessage.h"

using namespace std;
static const string tabLevel = "\t\t";

class XPLAction {
	public:
		explicit XPLAction(vector<XPLMessage>* responses);
		~XPLAction();
		void execute();
		bool equals(XPLAction* action);
		string printXML();

	protected:
		vector<XPLMessage>* getResponses();

	private:
		vector<XPLMessage>* responses_;
};

#endif //XPLAction_H
