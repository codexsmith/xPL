#include "XMLpugiXPL.h"

using namespace std;


createXPLCondition(vector<string> conditions)
createXPLAction(vector<XPLMessage>* messages)
createDeterminator(XPLCondition* condition, XPLAction* action, bool enabled)


Determinator* readDeterminator(string xmlString)
{

    const char* source = xmlString.c_str();
    size_t size = sizeof(source);
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_buffer(source, size);

    pugi::xml_node root = doc.document_element();//root

    for (pugi::xml_node_iterator root = tools.begin(); root != tools.end(); ++root)
    {
        std::cout << "Root:";

        for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
        {
            std::cout << " " << ait->name() << "=" << ait->value();
        }

        std::cout << std::endl;
    }

    return NULL;
}

