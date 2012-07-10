#ifndef DeterminatorFactory_H
#define DeterminatorFactory_H

#include <string>
#include <iostream>
#include "XPLHal.h"
#include "Determinator.h"

using namespace std;

const int startOfPairs = 2;
class DeterminatorFactory {
	public:
		DeterminatorFactory();
		~DeterminatorFactory();
		Determinator* createDeterminator(string defintions[]);

	private:
		XPLCondition* createCondition(string definitions[]);
		XPLAction* createAction(string defintions[]);
		string getMember(string definition);
		string getValue(string definition);
};

#endif //DeterminatorFactory_H
		
