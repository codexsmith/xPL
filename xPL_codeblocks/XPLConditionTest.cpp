#include <stdio.h>
#include <string>
#include "XPLCondition.h"

void resultCompare(int memberCompare, int valueCompare)
{
	if(memberCompare != 0 && valueCompare == 0)
	{
		printf("Members not equal\n");
	}
	else if(valueCompare != 0 && memberCompare == 0)
	{
		printf("Values not equal\n");
	}
	else if(memberCompare != 0 && valueCompare != 0)
	{
		printf("Member and Value compares failed\n");
	}
	else
	{
		printf("Test Success!\n");	
	}
}

int main()
{
	XPLValuePair* pairOne, pairTwo, pairThree;
	XPLMessage* message;
	message = new XPLMessage;
	pairOne = new XPLValuePair;
	pairTwo = new XPLValuePair;
	pairThree = new XPLValuePair;
//	pairOne->member = "nameOne";
//	pairOne->value  = "valueOne";
//	pairTwo->member = "nameTwo";
//	pairTwo->value  = "valueTwo";

//	int memberCompare = pairOne->member.compare("name");
//	int valueCompare = pairOne->value.compare("value");

//	resultCompare(memberCompare, valueCompare);

		

	return 0;		
}

