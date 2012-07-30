#include "XMLpugiXPL.h"

using namespace std;

DeterminatorFactory factory;

Determinator* readDeterminator(string xmlString)
{

    const char* source = xmlString.c_str();
    size_t size = sizeof(source);
    pugi::xml_document doc;

    vector<string> conditionsIn = vector<string>();
    vector<XPLAction> actionsIn = vector<XPLAction>();
    vector<XPLMessage> tmpMsgsIn = vector<XPLMessage>();

    string tmpStr;
    char* tmpChr;
    XPLMessage* tmpMsg;


    pugi::xml_parse_result result = doc.load_buffer(source, size);

    pugi::xml_node root = doc.document_element();//root

    pugi::xml_node conditions = root.child("xplCondition");
    pugi::xml_node actions = root.child("xplAction");
    string msg_type, srcAddress, dstAddress, schema;
    int hops;
    vector<string> parameters = vector<string>();

    for (pugi::xml_attribute_iterator ait = conditions.attributes_begin(); ait != conditions.attributes_end(); ++ait)
    {
        strcat(tmpChr,ait->name());
        strcat(tmpChr,"=");
        strcat(tmpChr, ait->value());
        tmpStr = string(tmpChr);
        conditionsIn.push_back(tmpStr);
    }

    msg_type = string(actions.attribute("msg_type").value());

    tmpMsg = factory.createXPLMessage(msg_type, srcAddress, dstAddress, schema, hops, parameters);
    tmpMsgsIn.push_back(*tmpMsg);


    actionsIn = factory.createXPLAction(&tmpMsgsIn);

    return NULL;
}

void printDeterminatorXML(string xmlString)
{
    const char* source = xmlString.c_str();
    size_t size = sizeof(source);
    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_buffer(source, size);

    pugi::xml_node root = doc.document_element();//root

    for (pugi::xml_node_iterator nid = root.begin(); nid != root.end(); ++nid)
    {
        std::cout << "Root:";

        for (pugi::xml_attribute_iterator ait = nid->attributes_begin(); ait != nid->attributes_end(); ++ait)
        {
            std::cout << " " << ait->name() << "=" << ait->value();
        }

        std::cout << std::endl;
    }

}
