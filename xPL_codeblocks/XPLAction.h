#ifndef XPLAction_H
#define XPLAction_H

#include <vector>
#include "XPLMessage.h"

using namespace std;

class XPLAction {
	public:
		explicit XPLAction(vector<XPLMessage>* responses);
		~XPLAction();
		vector<XPLMessage> execute();
		bool equals(XPLAction* action);

	protected:
		vector<XPLMessage>* getResponses();

	private:
		vector<XPLMessage>* responses_;
};

#endif //XPLAction_H
