#include "Determinator.h"
#include "XPLCondition.h"
#include "DeterminatorAction.h"

#include <vector>
#include <iostream>

//default constructor
Determinator::Determinator()
{
}

//Determinators should be constructed by the DeterminatorFactory only.
//User must have already created the appropriate condition and action.
//New Determinators are always enabled.
Determinator::Determinator( XPLCondition* condition, DeterminatorAction* action )
{
	condition_ = condition;
// 	action_ = action;
  actions.push_back(action);
	enabled_ = true;
}

Determinator::~Determinator()
{
}

// //Used for serialization in of determinators
// void Determinator::setAction(DeterminatorAction* action)
// {
//     action_ = action;
// }
// 
// DeterminatorAction* Determinator::getAction()
// {
// 	return action_;
// }

XPLCondition* Determinator::getCondition()
{
	return condition_;
}

string Determinator::getGUID()
{
	return GUID_;
}

void Determinator::setGUID(string GUID)
{
	GUID_ = GUID;
}

//Passthrough to XPLCondition to test if the message matches. 
//Always fails when the Determinator is disabled.
bool Determinator::match(XPLMessage* message)
{
	if(enabled_)
	{
		return condition_->match(message);
	}
	return false;
}

//pass through to XPLAction. When a determinator is matched, it executes its action.
//Future releases should add other action executions here.
void Determinator::execute()
{

    for (vector<DeterminatorAction*>::iterator dit = actions.begin(); dit != actions.end(); ++dit) {
            (*dit)->execute();
            cout<<"exec\n";
            flush(cout);
    }
	//action_->execute();
}

//Disabled determinators can never be executed. They are on by default.
void Determinator::setEnabled(bool enable)
{
	enabled_ = enable;
}

//Tests if this determinator can be executed or matched.
bool Determinator::isEnabled()
{
	return enabled_;
}


struct xml_string_writer: pugi::xml_writer
{
    std::string result;
    
    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
};


//Turns Determinator and its collaborators into formatted XML
//for serialization.
string Determinator::printXML()
{
    
    pugi::xml_document doc;
    //pugi::xml_node descr = doc.document_element().append_child("description");
    // add node with some name
    pugi::xml_node det = doc.append_child("determinator");
    det.append_attribute("guid") = "DEADBEEF";
    det.append_attribute("name") = "noname";
    det.append_attribute("description") = "";
    det.append_attribute("enabled") = "Y";
    
    pugi::xml_node inputnode = det.append_child("input");
    inputnode.append_attribute("match") = "any";
    condition_->appendCondition(&inputnode);
    
    pugi::xml_node outputnode = det.append_child("output");
    for (vector<DeterminatorAction*>::iterator dit = actions.begin(); dit != actions.end(); ++dit) {
        (*dit)->appendAction(&outputnode);
    }
/*    
    // add description node with text child
    pugi::xml_node descr = node.append_child("description");
    descr.append_child(pugi::node_pcdata).set_value("Simple node");
    
    // add param node before the description
    pugi::xml_node param = node.insert_child_before("param", descr);
    
    // add attributes to param node
    param.append_attribute("name") = "version";
    param.append_attribute("value") = 1.1;
    param.insert_attribute_after("type", param.attribute("name")) = "float";*/
    
    xml_string_writer xmlwrite;
    
    
    doc.save(xmlwrite);
    cout << xmlwrite.result;
    
    
    
	string result = "";
	result.append("<determinator ");
	result.append("guid=");
	result.append(GUID_);
	string enabled = "N";
	if(enabled_)
		enabled = "Y";
	result.append(" enabled=");
	result.append(enabled);
	result.append(">");
	result.append("\n\t<input match=\"any\">\n");
	string conditionXML = condition_->printXML();
	result.append(conditionXML);
	result.append("\n\t</input>\n");
	result.append("\n\t<output>\n");
	//string actionXML = action_->printXML();
	//result.append(actionXML);
	result.append("\n\t</output>\n");
	result.append("</determinator>");

	return result;
}
