#include "XPLAction.h"
#include "XPLCondition.h"
#include "XPLHal.h"

#include <iostream>
#include <string>

XPLValuePair* createFalsePair()
{
	XPLValuePair destructedPair;
	destructedPair.member = "destructedMember";
	destructedPair.value = "destructedValue";
	return &destructedPair;
}

XPLValuePair* createPairToInject()
{
	XPLValuePair* pairToInject = new XPLValuePair();
	pairToInject->member = "injectedMember";
	pairToInject->value = "injectedValue";
	return pairToInject;
}

void assertCorrect(XPLValuePair* pair_ptr, string value)
{
	if(pair_ptr->member.compare(value) == 0)
		cout << "Match Success: " << value << "\n";
	else
		cout << "Unable to match member.\n";
}
//
//int main()
//{
//	bool segfault = false;
//
//	if(segfault)
//	{
//		XPLValuePair* destructedPair = createFalsePair();
//		assertCorrect(destructedPair, "destructedMember");
//	}
//
//	XPLValuePair* injectedPair = createPairToInject();
//	assertCorrect(injectedPair, "injectedMember");
//
//	XPLValuePair copiedPair;
//	copiedPair.member = "copiedMember";
//	copiedPair.value = "copiedValue";
//
//	vector<XPLValuePair> pairVector;
//	pairVector.push_back(copiedPair);
//
//	copiedPair.member = "newMember";
//	copiedPair.value = "newValue";
//
//	XPLValuePair vectoredPair = pairVector.at(0);
//	cout << "Copied Member: " << vectoredPair.member << "\n";
//	cout << "Copied Value: " << vectoredPair.value << "\n";
//	cout << "New Member: " << copiedPair.member << "\n";
//	cout << "New Value: " << copiedPair.value << "\n";
//
//	cout << &copiedPair << "\n";
//	cout << &vectoredPair << "\n";
//	return 0;
//}
