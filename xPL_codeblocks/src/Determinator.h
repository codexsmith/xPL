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
        explicit Determinator();
        explicit Determinator( XPLCondition* condition, DeterminatorAction* action );
        explicit Determinator( string );
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
        vector<XPLCondition*> conditions;
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
