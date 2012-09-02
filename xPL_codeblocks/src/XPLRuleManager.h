#ifndef XPLRuleManager_H
#define XPLRuleManager_H

#include "XPLMessage.h"
#include <vector>
#include "Determinator.h"

using namespace std;

class XPLRuleManager {
	public:
        XPLRuleManager();
        XPLRuleManager(std::vector< Determinator* >* );
		~XPLRuleManager();
		void match(XPLMessage);
        std::string detToString();//XHCP support
        Determinator* retrieveDeterminator(string GUID);
	void saveDeterminators();
	
	private:
        vector<Determinator*>* determinators;
        static const string saveLocation ;
        int makeDeterminatorDir();
        
        void loadDeterminators( vector< Determinator*>*);
};

#endif //XPLRuleManager_H
