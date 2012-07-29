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
		Determinator* createDeterminator(XPLCondition* condition, XPLAction* action, bool enabled);
		XPLMessage* createXPLMessage(string msgType, string sourceAddress, string destinationAddress, string schema, int hops, vector<string> parameters);

		XPLCondition* createXPLCondition(vector<string> definitions, string source, string destination, string schema, string hops, string msgType);
		XPLAction* createXPLAction(vector<XPLMessage>* messages);
		string getMember(string definition);
		string getValue(string definition);
		vector<string> getAddressParameters(string address);

	private:
		string getSchemaClass(string definition);
		string getSchemaType(string definition);
		
};

#endif //DeterminatorFactory_H
		
