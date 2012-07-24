#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "XPLCondition.h"
#include "XPLMessage.h"


class XPLConditionTest {

public:
	
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

	vector<XPLValuePair>* createPairVector()
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

		vector<XPLValuePair>* pairVector = createVector(pairOne, pairTwo, pairThree);	

		return pairVector;
	}

	void testMatch()
	{
		vector<XPLValuePair>* pairVector = createPairVector();	
		vector<XPLValuePair>* pairVectorTwo = createPairVector();

		XPLCondition* conditionOne;
		conditionOne = new XPLCondition(pairVector);

		XPLCondition* conditionTwo;
		conditionTwo = new XPLCondition(pairVectorTwo);

		XPLMessage messageOne;
		messageOne.addMember("nameOne", "valueOne");
		messageOne.addMember("nameTwo", "valueTwo");
		messageOne.addMember("nameThree", "valueThree");

		XPLMessage messageTwo;
		messageOne.addMember("nameOne", "valueTwo");
		messageOne.addMember("nameTwo", "valueOne");

		bool matched = conditionOne->match(&messageOne);
		bool notMatched = conditionTwo->match(&messageTwo);

		if( matched && !notMatched )
			printf("Test XPLCondition::match() success!\n");
		else
		{
			printf("Test XPLCondition::match() failed!\n");
			cout << "\tMatched Result: " << matched << "\n\t Not Matchd Result: " << notMatched << "\n";
		}
	}

	void testEquals()
	{
		vector<XPLValuePair>* pairVector = createPairVector();	
		vector<XPLValuePair>* pairVectorTwo = createPairVector();

		XPLCondition* conditionOne;
		conditionOne = new XPLCondition(pairVector);

		XPLCondition* conditionTwo;
		conditionTwo = new XPLCondition(pairVectorTwo);

		if(conditionOne->equals(conditionTwo))
			printf("Test XPLCondition::equals() success!\n");
		else
		{
			printf("Test XPLCondition::equals() failed!\n");
		}
	}

	void runTests()
	{
		testMatch();		
	}
};
