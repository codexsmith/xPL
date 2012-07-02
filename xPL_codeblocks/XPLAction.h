#ifndef XPLAction_H
#define XPLAction_H

#include "XPLMessage.h"

class XPLAction {
	public:
		XPLAction(vector<XPLMessage> responses);
		~XPLAction();
		vector<XPLMessage> execute();

	private:
		vector<XPLMessage> responses_;
};

#endif //XPLAction_H
