#include <iostream>
#include "DeterminatorSerializer.h"

using namespace std;

class DeterminatorSerializerTest {
public:

    void assertSuccess(string functionName, bool result, string value)
	{
		if(result)
			cout << functionName +" Success!\n Got:" << value << "\n";
		else
			cout << functionName +" failed!\n Got: " << value << "\n";
	}

    void testReadDeterminator(DeterminatorSerializer* parser)
    {
        string ruleIn = parser->read();

        Determinator* rule = parser->parseDeterminator(ruleIn);

        assertSuccess("testReadDeterminator", ruleIn.compare("") != 0, ruleIn);

    }

    void runTests()
    {

        DeterminatorSerializer serializer = DeterminatorSerializer("determinatorSample.xml");
        testReadDeterminator(&serializer);

    }

};
