#ifndef XPLRuleManager_H
#define XPLRuleManager_H

#include <vector>
#include "XPLMessage.h"
#include "Determinator.h"

using namespace std;

class XPLRuleManager {
	public:
        XPLRuleManager();
        XPLRuleManager(vector<Determinator>);
		~XPLRuleManager();
		vector<XPLMessage> match(XPLMessage);

	private:
        vector<Determinator> determinators;
};

#endif //XPLRuleManager_H
