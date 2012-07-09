#include <stdio.h>
#include <string>
#include <vector>
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

vector<XPLValuePair>* createVector(XPLValuePair* pairOne, XPLValuePair* pairTwo, XPLValuePair* pairThree)
{
	vector<XPLValuePair>* pairVector;
	pairVector = new vector<XPLValuePair>();
	pairVector->push_back(*pairOne);
	pairVector->push_back(*pairTwo);
	pairVector->push_back(*pairThree);

	return pairVector;
}

int main()
{
	XPLValuePair* pairOne; 
	XPLValuePair* pairTwo; 
	XPLValuePair* pairThree;

	pairOne = new XPLValuePair();
	pairTwo = new XPLValuePair();
	pairThree = new XPLValuePair();
	
	pairOne->member = "nameOne";
	pairOne->value  = "valueOne";
	pairTwo->member = "nameTwo";
	pairTwo->value  = "valueTwo";
	pairThree->member = "nameThree";
	pairThree->value = "valueThree";

	int memberCompare = pairOne->member.compare("nameOne");
	int valueCompare = pairOne->value.compare("valueOne");
	resultCompare(memberCompare, valueCompare);

	memberCompare = pairTwo->member.compare("nameTwo");
	valueCompare = pairTwo->value.compare("valueTwo");
	resultCompare(memberCompare, valueCompare);

	memberCompare = pairThree->member.compare("nameThree");
	valueCompare = pairThree->value.compare("valueThree");
	resultCompare(memberCompare, valueCompare);

	
	vector<XPLValuePair>* pairVector = createVector(pairOne, pairTwo, pairThree);	
	vector<XPLValuePair>* pairVectorTwo = createVector(pairOne, pairTwo, pairThree);

	XPLCondition* conditionOne;
	conditionOne = new XPLCondition(pairVector);

	XPLCondition* conditionTwo;
	conditionTwo = new XPLCondition(pairVectorTwo);

	if(conditionOne->equals(conditionTwo))
		printf("Conditions equal!\n");
	else
		printf("Conditions not equal\n");

	return 0;		
}

