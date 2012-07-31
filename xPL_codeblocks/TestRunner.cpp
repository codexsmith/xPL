#include "XPLConditionTest.cpp"
#include "DeterminatorTest.cpp"
#include "XPLActionTest.cpp"
#include "XPLMessageTest.cpp"
#include "XPLRuleManager.h"
#include "DeterminatorSerializerTest.cpp"

extern "C" {

    #include "xPLLib/xPL.h"

}

xPL_ServicePtr theService = NULL;
XPLRuleManager* ruleMgr;

int main()
{
//
//	XPLConditionTest xplConditionTest;
//	xplConditionTest.runTests();
//
//	DeterminatorTest determinatorTest;
//	determinatorTest.runTests();
//
//	XPLActionTest xplActionTest;
//	xplActionTest.runTests();
//
//    XPLMessageTest::runTests();

    DeterminatorSerializerTest ruleSerializerTest;
    ruleSerializerTest.runTests();

	return 0;
}
