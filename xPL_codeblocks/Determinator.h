#ifndef Determinator_H
#define Determinator_H
#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLMessage.h"

class Determinator {
	public:
		Determinator(XPLCondition condition, XPLAction action);
		~Determinator();
		bool match(XPLMessage message);
		vector<XPLMessage> execute();	
		void setEnabled(bool status);
		bool isEnabled();

	private:
		XPLCondition condition_;
		XPLAction action_;
		bool enabled_;
};

#endif //Determinator_H
