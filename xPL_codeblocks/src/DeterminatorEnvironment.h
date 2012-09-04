#ifndef DeterminatorEnvrionment_H
#define DeterminatorEnvrionment_H

#include "XPLMessage.h"
#include <time.h>

using namespace std;

class DeterminatorEnvrionment {
	public:
        DeterminatorEnvrionment();
        DeterminatorEnvrionment(XPLMessage*);
	//~XPLRuleManager();
	
	XPLMessage* message;
	

        };

#endif //DeterminatorEnvrionment_H
