#include <iostream>
#include "DeterminatorSerializer.h"

class DeterminatorSerializerTest {

public:

	void assertSuccess(string functionName, bool result, string value)
	{
		if(result)
			cout << functionName +" Success!\n Got:" << value << "\n";
		else
			cout << functionName +" failed!\n Got: " << value << "\n";
	}

	void testParseDeterminator()
	{
		DeterminatorSerializer serializer("notARealFile");
		string xmlString = "<determinator\nguid=\"rule-guid\"\nname=\"rule-name\"\ndescription=\"rule-description\"\nenabled=\"Y|N\"\ngroupName=\"\" \nisGroup=\"N\">\n<input match=\"all|any\">\n<xplCondition display_name=\"condition-name\"\nmsg_type=\"cmnd|stat|trig|*\"\nsource_vendor=\"vendor-name|*\"\nsource_device=\"device-name|*\"\nsource_instance=\"instance-name|*\"\ntarget_vendor=\"vendor-name|*\"\ntarget_device=\"device-name|*\"\ntarget_instance=\"instance-name|*\"\nschema_class=\"schema-class|*\"\nschema_type=\"schema-type|*\">\n<param\nname=\"parameter-name\"\noperator=\"=|!=|>|<|>=|<=\"\nvalue=\"parameter-value\"\n/>\n</xplCondition>\n</input>\n<output>\n<xplAction display_name=\"action-name\"\nexecuteOrder=\"nnn\"\nmsg_type=\"stat|trig|cmnd\"\nmsg_target=\"*|v-d.i\"\nmsg_schema=\"class.type\">\n<xplActionParam\nexpression=\"name=value\"\n/>\n</xplAction>\n</output>\n</determinator>";

		cout << xmlString;
		Determinator* determinator = serializer.parseDeterminator(xmlString);
		string writtenDeterminator = serializer.
		
	}


	void runTests()
	{
		testParseDeterminator();
	}
};
