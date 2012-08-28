#ifndef XPLRuleManager_H
#define XPLRuleManager_H

#include "XPLMessage.h"
#include <vector>
#include "Determinator.h"

using namespace std;

class XPLRuleManager {
	public:
        XPLRuleManager();
        XPLRuleManager(vector<Determinator>*);
		~XPLRuleManager();
		void match(XPLMessage);
        std::string detToString();//XHCP support
        Determinator* retrieveDeterminator(string GUID);
	private:
        vector<Determinator>* determinators;
};

#endif //XPLRuleManager_H
