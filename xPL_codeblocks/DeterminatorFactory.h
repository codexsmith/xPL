#ifndef DeterminatorFactory_H
#define DeterminatorFactory_H

#include <string>
#include <iostream>
#include <vector>
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
		Determinator* createDeterminator(vector<string> determinator, vector<string> condition, vector<string> action);
		XPLMessage* createXPLMessage(string msgType, string sourceAddress, string destinationAddress, string schema, int hops, vector<string> parameters);

		XPLCondition* createCondition(vector<string> definitions);
		XPLAction* createXPLAction(vector<XPLMessage>* messages);
		string getMember(string definition);
		string getValue(string definition);
		vector<string> getAddressParameters(string address);

	private:
		
};

#endif //DeterminatorFactory_H
		
