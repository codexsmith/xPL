#ifndef XPLCondition_H
#define XPLCondition_H

#include "XPLHal.h"
#include "XPLMessage.h"

using namespace std;

class XPLCondition {
	public:
		explicit XPLCondition(vector<XPLValuePair>* attributes);
		~XPLCondition();
		bool match(XPLMessage* message);
		bool equals(XPLCondition* condition);

	protected:
		vector<XPLValuePair>* getAttributes();

	private:
		vector<XPLValuePair>* attributes_;
};

#endif //XPLAction_H
