#ifndef Determinator_H
#define Determinator_H

#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"

#include <vector>

using namespace std;

class Determinator {
	public:
        Determinator();
		Determinator(XPLCondition* condition, XPLAction* action);
		~Determinator();
		bool match(XPLMessage* message);
		void execute();
		void setEnabled(bool enable);
		bool isEnabled();
        void setAction(XPLAction* action);
        string printXML();
        void setGUID(string GUID);
        string getGUID();

	protected:
		XPLCondition* getCondition();
		XPLAction* getAction();

	private:
		string GUID_;
		XPLCondition* condition_;
		XPLAction* action_;
		bool enabled_;
};

#endif //Determinator_H
