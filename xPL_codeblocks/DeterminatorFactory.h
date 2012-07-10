#ifndef DeterminatorFactory_H
#define DeterminatorFactory_H

#include <string>
#include <iostream>
#include "XPLHal.h"
#include "Determinator.h"

using namespace std;

//Following the format of the determinator command, the third item
//will be the first valuePair to be defined in its condition

const int startOfPairs = 2; 

class DeterminatorFactory {
	public:
		DeterminatorFactory();
		~DeterminatorFactory();
		Determinator* createDeterminator(string defintions[]);

		XPLCondition* createCondition(string definitions[]);
		XPLAction* createAction(string defintions[]);
		string getMember(string definition);
		string getValue(string definition);

	private:
		
};

#endif //DeterminatorFactory_H
		
