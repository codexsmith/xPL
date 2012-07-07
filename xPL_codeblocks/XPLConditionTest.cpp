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
	pairOne = new XPLValuePair;
	
	pairOne->member = "nameOne";
	pairOne->value  = "valueOne";
	pairTwo->member = "nameTwo";
	pairTwo->value  = "valueTwo";
	pairThree->member = "nameThree";
	pairThree->value = "nameThree";

	int memberCompare = pairOne->member.compare("nameOne");
	int valueCompare = pairOne->value.compare("valueOne");
	resultCompare(memberCompare, valueCompare);

	int memberCompare = pairTwo->member.compare("nameTwo");
	int valueCompare = pairTwo->value.compare("valueTwo");
	resultCompare(memberCompare, valueCompare);

	int memberCompare = pairThree->member.compare("nameThree");
	int valueCompare = pairThree->value.compare("valueThree");
	resultCompare(memberCompare, valueCompare);

	return 0;		
}

