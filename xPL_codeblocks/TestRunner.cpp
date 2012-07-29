#include "DeterminatorFactoryTest.cpp"
#include "XPLConditionTest.cpp"
#include "DeterminatorTest.cpp"
#include "XPLActionTest.cpp"
#include "XPLMessageTest.cpp"
#include "XPLRuleManager.h"

extern "C" {

    #include "xPLLib/xPL.h"

}

int main()
{
	DeterminatorFactoryTest determinatorFactoryTest;
	determinatorFactoryTest.runTests();

	XPLConditionTest::runTests();

	DeterminatorTest determinatorTest;
	determinatorTest.runTests();

	XPLActionTest xplActionTest;
	xplActionTest.runTests();

    XPLMessageTest::runTests();

	return 0;
}

