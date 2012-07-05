#ifndef Determinator_H
#define Determinator_H

#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"

using namespace std;

class Determinator {
	public:
		Determinator(XPLCondition condition, XPLAction action);
		Determinator(Determinator* determinator);
		~Determinator();
		bool match(XPLMessage message);
<<<<<<< HEAD
		vector<XPLMessage> execute();	
		void setEnabled(bool enable);
=======
		vector<XPLMessage> execute();
		void setEnabled(bool status);
>>>>>>> 5cddd874eec6ce6c3e0e4a946078298fe143d419
		bool isEnabled();
		XPLAction getAction();
		XPLCondition getCondition();

	protected:
		XPLCondition getCondition();
		XPLAction getAction();

	private:
		void operator=(const Determinator&);
		XPLCondition condition_;
		XPLAction action_;
		bool enabled_;
};

#endif //Determinator_H
