#ifndef XPLAction_H
#define XPLAction_H

#include <vector>
#include "XPLMessage.h"

using namespace std;

class XPLAction {
	public:
		explicit XPLAction(vector<XPLMessage> responses);
		~XPLAction();
		vector<XPLMessage> execute();

	private:
		void operator=(const XPLAction&);
		vector<XPLMessage> responses_;
};

#endif //XPLAction_H
