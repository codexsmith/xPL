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
		XPLAddress sourceAddressOne;
		sourceAddressOne.device = "device";
		sourceAddressOne.vendor = "vendor";
		sourceAddressOne.instance = "instance";
		XPLAddress destinationAddressOne;
		destinationAddressOne.device = "destDevice";
		destinationAddressOne.vendor = "destVendor";
		destinationAddressOne.instance = "destInstance";
		XPLSchema schemaOne;
		schemaOne.schema = "schema";
		schemaOne.type = "type";
		int hopsOne = 5;
		string msgTypeOne = "xpl-cmd";

		vector<XPLValuePair>* pairVectorTwo = createPairVector();
		XPLAddress sourceAddressTwo;
		sourceAddressTwo.device = "device";
		sourceAddressTwo.vendor = "vendor";
		sourceAddressTwo.instance = "instance";
		XPLAddress destinationAddressTwo;
		destinationAddressTwo.device = "destDevice";
		destinationAddressTwo.vendor = "destVendor";
		destinationAddressTwo.instance = "destInstance";
		XPLSchema schemaTwo;
		schemaTwo.schema = "schema";
		schemaTwo.type = "type";
		int hopsTwo = 5;
		string msgTypeTwo = "xpl-cmd";

		XPLCondition* conditionOne;
		conditionOne = new XPLCondition(pairVector, sourceAddressOne, destinationAddressOne, schemaOne, hopsOne, msgTypeOne);

		XPLCondition* conditionTwo;
		conditionTwo = new XPLCondition(pairVectorTwo, sourceAddressTwo, destinationAddressTwo, schemaTwo, hopsTwo, msgTypeTwo);

		XPLMessage messageOne;
		XPLAddress messageOneSource;
		messageOneSource.vendor = "vendor";
		messageOneSource.device = "device";
		messageOneSource.instance = "instance";
		messageOne.setSource(messageOneSource);
		messageOne.addMember("nameOne", "valueOne");
		messageOne.addMember("nameTwo", "valueTwo");
		messageOne.addMember("nameThree", "valueThree");

		XPLMessage messageTwo;
		XPLAddress messageTwoSource;
		messageTwoSource.vendor = "vendor";
		messageTwoSource.device = "device";
		messageTwoSource.instance = "instance";
		messageTwo.setSource(messageTwoSource);
		messageTwo.addMember("nameOne", "valueTwo");
		messageTwo.addMember("nameTwo", "valueOne");

		bool matched = conditionOne->match(&messageOne);
		bool notMatched = conditionTwo->match(&messageTwo);

		if( matched && !notMatched )
			printf("Test XPLCondition::match() success!\n");
		else
		{
			printf("Test XPLCondition::match() failed!\n");
			cout << "\tMatched Result: " << matched << "\n\t Not Matched Result: " << notMatched << "\n";
		}
	}

	void testEquals()
	{
		vector<XPLValuePair>* pairVector = createPairVector();
		XPLAddress sourceAddressOne;
		sourceAddressOne.device = "device";
		sourceAddressOne.vendor = "vendor";
		sourceAddressOne.instance = "instance";
		XPLAddress destinationAddressOne;
		destinationAddressOne.device = "destDevice";
		destinationAddressOne.vendor = "destVendor";
		destinationAddressOne.instance = "destInstance";
		XPLSchema schemaOne;
		schemaOne.schema = "schema";
		schemaOne.type = "type";
		int hopsOne = 5;
		string msgTypeOne = "xpl-cmd";

		vector<XPLValuePair>* pairVectorTwo = createPairVector();
		XPLAddress sourceAddressTwo;
		sourceAddressTwo.device = "device";
		sourceAddressTwo.vendor = "vendor";
		sourceAddressTwo.instance = "instance";
		XPLAddress destinationAddressTwo;
		destinationAddressTwo.device = "destDevice";
		destinationAddressTwo.vendor = "destVendor";
		destinationAddressTwo.instance = "destInstance";
		XPLSchema schemaTwo;
		schemaTwo.schema = "schema";
		schemaTwo.type = "type";
		int hopsTwo = 5;
		string msgTypeTwo = "xpl-cmd";

		XPLCondition* conditionOne;
		conditionOne = new XPLCondition(pairVector, sourceAddressOne, destinationAddressOne, schemaOne, hopsOne, msgTypeOne);

		XPLCondition* conditionTwo;
		conditionTwo = new XPLCondition(pairVectorTwo, sourceAddressTwo, destinationAddressTwo, schemaTwo, hopsTwo, msgTypeTwo);

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
		testEquals();		
	}
};
