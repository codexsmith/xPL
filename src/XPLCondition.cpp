#include "XPLCondition.h"
#include "XPLCommon.h"

#include "Determinator.h"
#include <string>
#include <iostream>
#include <string.h>

using namespace std;

//XPLConditions should only be constructed in an appropriate factory. Improper values may cause
//matching issues. All XPLMessage header parameters are stored in their own fields, while body
//parameters are passed in via a vector of XPLValuePairs.
XPLCondition::XPLCondition(vector<XPLValuePair>* attributes, XPLAddress sourceAddress, XPLAddress destinationAddress, XPLSchema schema, int hops, string msgType)
{
	msgType_ = msgType;
	sourceAddress_ = sourceAddress;
	destinationAddress_ = destinationAddress;
	schema_ = schema;
	hops_ = hops;
	attributes_ =  *attributes;
}

XPLCondition::XPLCondition() {
    
    msgType_="cmnd";
    schema_.schema="control";
    schema_.type="basic";
    hops_=3;
    XPLValuePair* pairThree = new XPLValuePair();
    XPLValuePair* pairFour = new XPLValuePair();
    pairThree->member = "device";
    pairThree->value = "pwm";
    pairFour->member = "current";
    pairFour->value = "1";
    attributes_.push_back(*pairThree);
    attributes_.push_back(*pairFour);
}


XPLCondition::XPLCondition(pugi::xml_node condnode) {
    
    bool failed = false;
    //attributes_ = new vector<XPLValuePair>();
    
    if (condnode.attribute("display_name")) {
        display_name = condnode.attribute("display_name").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("msg_type")) {
        msgType_ = condnode.attribute("msg_type").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("schema_class")) {
        schema_.schema = condnode.attribute("schema_class").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("schema_type")) {
        schema_.type = condnode.attribute("schema_type").as_string();
    } else {
        failed = true;
    }
    
    if (condnode.attribute("source_vendor")) {
        sourceAddress_.vendor= condnode.attribute("source_vendor").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("source_device")) {
        sourceAddress_.device= condnode.attribute("source_device").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("source_instance")) {
        sourceAddress_.instance= condnode.attribute("source_instance").as_string();
    } else {
        failed = true;
    }
    
    
    if (condnode.attribute("dest_vendor")) {
        destinationAddress_.vendor= condnode.attribute("dest_vendor").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("dest_device")) {
        destinationAddress_.device= condnode.attribute("dest_device").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("dest_instance")) {
        destinationAddress_.instance= condnode.attribute("dest_instance").as_string();
    } else {
        failed = true;
    }

 
    for (pugi::xml_node_iterator ait = condnode.begin(); ait != condnode.end(); ++ait)
    {
        if (!strcmp(ait->name(), "param")) {
            if ((*ait).attribute("name") && (*ait).attribute("operator") && (*ait).attribute("value")) {
                
                string mname = (*ait).attribute("name").as_string();
                string moper = Determinator::unescape((*ait).attribute("operator").as_string());
                string mvalue = (*ait).attribute("value").as_string();
                if(moper == "="){
                    XPLValuePair* pair = new XPLValuePair();  //FIXME this appears to not get cleaned up??
                    pair->member = mname;
                    pair->value = mvalue;
		    //cout << "\t added pair: " << pair << "\n";
                    attributes_.push_back(*pair);
		    delete pair;
                }
            } else {
                failed = true;
            } 
        }
    }
    //cout << "\t\tloaded " << attributes_.size() << " attributes\n";
}




//Matches a message. Returns true when all of the CONDITION'S parameters have
//matching values in the passed in message.
bool XPLCondition::match(DeterminatorEnvironment* env)
{
    //if the determinator env wasn't created due to a message comming in, don't match.
    if(env->envType != DeterminatorEnvironment::xPLMessage) {
        return false;
    }
    AutoPtr<xplMsg> message = env->message;
    
	XPLAddress sourceAddress = message->GetSource();
	XPLAddress destinationAddress = message->GetTarget();
  string mType = message->GetType();
  if(mType.find_first_of("xpl-")==0) {
      mType = mType.substr(mType.find_first_of("xpl-")+4);
  }
  int hops = message->GetHop();

  Logger& condlog = Logger::get("xplCondition");
  condlog.trace("source " + sourceAddress_.device + "  " + sourceAddress.device);
  condlog.trace("dest " + destinationAddress_.device + "  " + destinationAddress.device);
  condlog.trace("msg " + mType + "  " + msgType_);
  
  bool msgMatch = (msgType_.compare(mType) == 0);
//	bool hopsMatch = (hops_ == hops) || hops_ == NULL;
	bool sourceMatch = (sourceAddress_.vendor.compare(sourceAddress.vendor) == 0) && (sourceAddress_.device.compare(sourceAddress.device) == 0) && (sourceAddress_.instance.compare(sourceAddress.instance) == 0);
	bool destinationMatch = (destinationAddress_.vendor.compare(destinationAddress.vendor) == 0) && (destinationAddress_.device.compare(destinationAddress.device) == 0) && (destinationAddress_.instance.compare(destinationAddress.instance) == 0);
	bool membersMatch = true;
  
	for(int i = 0; i < attributes_.size(); i++)
	{

		XPLValuePair memberToFind = attributes_.at(i);
		//string value = message->findMember(memberToFind.member);
    const xplMsgItem* itemp = message->GetMsgItem(memberToFind.member);
    if(!itemp) {
        //cout<<"\t\tXPL cond testing: " << memberToFind.member << " : false\n";
        membersMatch = false;
        break;
    }

    if(!(memberToFind.value.compare(itemp->GetValue(0)) == 0))
		{
        //cout<<"\t\tXPL cond testing: " << memberToFind.member << " =  " << itemp->GetValue(0) << " : false\n";
			membersMatch = false;
			break;
		}
		//cout<<"\t\tXPL cond testing: " << memberToFind.member << " =  " << itemp->GetValue(0) << " : true\n";
	}
	//cout << "cond matched: " << membersMatch << "\n";
	return (msgMatch && sourceMatch && destinationMatch && membersMatch);
}

//Compares conditions based on the equivalencey of each of their member variables.
bool XPLCondition::equals(XPLCondition* condition)
{
	vector<XPLValuePair>* compareTo = condition->getAttributes();
	if(compareTo->size() != attributes_.size())
		return false;
	for(int i = 0; i < attributes_.size(); i++)
	{
		XPLValuePair* attribute = &attributes_.at(i);
		XPLValuePair* compare = &compareTo->at(i);
		if(attribute->member.compare(compare->member) != 0)
			return false;
		if(attribute->value.compare(compare->value) != 0)
			return false;
	}
	return true;
}

vector<XPLValuePair>* XPLCondition::getAttributes()
{
	return &attributes_;
}

//Turns the XPLCondition into a formatted XML string
//for serialization.



void XPLCondition::appendCondition(pugi::xml_node* inputnode) {
    
    pugi::xml_node condnode = inputnode->append_child("xplCondition");
    
    condnode.append_attribute("display_name") = "test";
    condnode.append_attribute("msg_type") = msgType_.c_str();
    
    condnode.append_attribute("source_vendor") = sourceAddress_.vendor.c_str();
    condnode.append_attribute("source_device") = sourceAddress_.device.c_str();
    condnode.append_attribute("source_instance") = sourceAddress_.instance.c_str();
    condnode.append_attribute("target_vendor") = destinationAddress_.vendor.c_str();
    condnode.append_attribute("target_device") = destinationAddress_.device.c_str();
    condnode.append_attribute("target_instance") = destinationAddress_.instance.c_str();
    condnode.append_attribute("schema_class") = schema_.schema.c_str();
    condnode.append_attribute("schema_type") = schema_.type.c_str();
    
    for(int i = 0; i<attributes_.size(); i++)
    {
        pugi::xml_node paramnode = condnode.append_child("param");
        paramnode.append_attribute("name") =  attributes_.at(i).member.c_str();
        paramnode.append_attribute("operator") =  "=";
        paramnode.append_attribute("value") =  attributes_.at(i).value.c_str();
    }
    
}

string XPLCondition::printXML()
{

    
	string result = "";
	result.append("\t<xplcondition");
	result.append("\n\tmsg_type=");
	result.append(msgType_);
	result.append("\n\tsource_vendor=");
	result.append(sourceAddress_.vendor);
	result.append("\n\tsource_device=");
	result.append(sourceAddress_.device);
	result.append("\n\tsource_instance=");
	result.append(sourceAddress_.instance);
	result.append("\n\ttarget_vendor=");
	result.append(destinationAddress_.vendor);
	result.append("\n\ttarget_device=");
	result.append(destinationAddress_.device);
	result.append("\n\ttarget_instance=");
	result.append(destinationAddress_.instance);
	result.append("\n\tschema_class=");
	result.append(schema_.schema);
	result.append("\n\tschema_type=");
	result.append(schema_.type);
	result.append("\n\t>\n");

	for(int i = 0; i<attributes_.size(); i++)
	{
		result.append("\t\t<param name=" + attributes_.at(i).member + "\n");
		result.append("\t\toperator=\n");
		result.append(tabs + "value=" + attributes_.at(i).value + "\n");
		result.append(tabs + ">\n");
	}
	result.append("</xplcondition>\n");
	return result;
}
