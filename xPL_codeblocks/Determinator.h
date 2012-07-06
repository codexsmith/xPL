#ifndef Determinator_H
#define Determinator_H

#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"

using namespace std;

class Determinator {
	public:
		Determinator(XPLCondition* condition, XPLAction* action);
		Determinator(Determinator* determinator);
		~Determinator();
		bool match(XPLMessage* message);
		vector<XPLMessage> execute();	
		void setEnabled(bool enable);
		bool isEnabled();

	protected:
		XPLCondition* getCondition();
		XPLAction* getAction();

	private:
		void operator=(const Determinator&);
		XPLCondition* condition_;
		XPLAction* action_;
		bool enabled_;
};

#endif //Determinator_H
