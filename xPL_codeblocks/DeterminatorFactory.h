#ifndef DeterminatorFactory_H
#define DeterminatorFactory_H

#include <string>
#include <iostream>
#include "XPLHal.h"
#include "Determinator.h"
#include "XPLMessage.h"

using namespace std;

//Following the format of the determinator command, the third item
//will be the first valuePair to be defined in its condition

class DeterminatorFactory {
	public:
		DeterminatorFactory();
		~DeterminatorFactory();
		Determinator* createDeterminator(string defintions[]);
		XPLMessage* createXPLMessage(string msgType, string destinationAddress, string sourceAddress, string schema, int hops, string parameters[]);

		XPLCondition* createCondition(string definitions[]);
		XPLAction* createAction(XPLMessage messages[]);
		string getMember(string definition);
		string getValue(string definition);
		string* getAddressParameters(string address);

	private:
		
};

#endif //DeterminatorFactory_H
		
