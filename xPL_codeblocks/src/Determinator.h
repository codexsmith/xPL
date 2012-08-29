#ifndef Determinator_H
#define Determinator_H

#include "DeterminatorAction.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"

#include <vector>

using namespace std;

class Determinator {
	public:
        Determinator();
        Determinator( XPLCondition* condition, DeterminatorAction* action );
        Determinator( string );
		~Determinator();
		bool match(XPLMessage* message);
		void execute();
		void setEnabled(bool enable);
		bool isEnabled();
//     void setAction(DeterminatorAction* action);
        string printXML();
        void setGUID(string GUID);
        string getGUID();
        vector<DeterminatorAction*> actions;
        
	protected:
		XPLCondition* getCondition();
//     DeterminatorAction* getAction();

	private:
		string GUID_;
    string name;
    string description;
		XPLCondition* condition_;
//     DeterminatorAction* action_;
		bool enabled_;
};

#endif //Determinator_H
