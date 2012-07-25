#include "DeterminatorFactoryTest.cpp"
#include "XPLConditionTest.cpp"
#include "DeterminatorTest.cpp"
#include "XPLActionTest.cpp"

int main()
{
	DeterminatorFactoryTest determinatorFactoryTest;
	determinatorFactoryTest.runTests();

	XPLConditionTest xplConditionTest;
	xplConditionTest.runTests();

	DeterminatorTest determinatorTest;
	determinatorTest.runTests();

	XPLActionTest xplActionTest;
	xplActionTest.runTests();

	return 0;
}