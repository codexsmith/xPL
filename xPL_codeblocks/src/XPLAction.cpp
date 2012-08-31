#include <vector>
#include <string>
#include <iostream>

#include "XPLAction.h"
#include "XPLMessage.h"
#include "XPLParser.h"
#include "DeterminatorFactory.h"

using namespace std;

//XPLActions are directly constructed their vector of messages.
//Should only be used in an appropriate factory.
XPLAction::XPLAction(vector<XPLMessage>* responses)
{
	responses_ = responses;
  message = (*responses)[0];
}

XPLAction::XPLAction(pugi::xml_node actionnode)
{
 
    bool failed = false;
    
    if (actionnode.attribute("display_name")) {
        display_name = actionnode.attribute("display_name").as_string();
    } else {
        failed = true;
    }
    if (actionnode.attribute("executeOrder")) {
        executeOrder = actionnode.attribute("executeOrder").as_int();
    } else {
        failed = true;
    }
    if (actionnode.attribute("msg_type")) {
        message.setMsgType(actionnode.attribute("msg_type").as_string());
    } else {
        failed = true;
    }
    
    if (actionnode.attribute("msg_target")) {
        string target = actionnode.attribute("msg_target").as_string();
        if (target == "*"){
            message.setBroadcast(true);
        } else {
            XPLAddress dest;
            DeterminatorFactory df;
            vector<string> parameters = df.getAddressParameters(target);
            dest.vendor = parameters[0];
            dest.device = parameters[1];
            dest.instance = parameters[2];
            message.setDestination(dest);
            message.setBroadcast(false);
        }
    } else {
        failed = true;
    }
    
    if (actionnode.attribute("msg_schema")) {
        string schema = actionnode.attribute("msg_schema").as_string();
        int schemaSplit = schema.find(".");
        string schemaClass = schema.substr(0, schemaSplit);
        string schemaType = schema.substr(schemaSplit+1, schema.length() - schemaSplit);
        message.setSchema(schemaClass, schemaType);
    } else {
        failed = true;
    }
    
    
   /* 
    if (detnode.attribute("msg_target")) {
        if (detnode.attribute("enabled").as_string() == "Y") enabled_=true;
        else enabled_ = false;
    } else {
        failed = true;
    }
    */

   //pugi::xml_node outnode =actionnode.child("output"); 
   for (pugi::xml_node_iterator ait = actionnode.begin(); ait != actionnode.end(); ++ait)
    {
        //cout << "m: " << ait->name() << ". \n";
        if (!strcmp(ait->name(), "xplActionParam")) {
            if ((*ait).attribute("expression")) {
                string expr = (*ait).attribute("expression").as_string();
                int exprsplit = expr.find("=");
                string member = expr.substr(0, exprsplit);
                string value = expr.substr(exprsplit+1, expr.length() - exprsplit);
                
                message.addMember(member, value);
            } else {
                failed = true;
            } 
        }
    }
    cout << "\t\tloaded " << message.getMembers().size() << " members\n";
    
    
}

XPLAction::~XPLAction()
{
	delete responses_;
}

//Method to execute the determinator.
//@return returns a vector of XPLMessages for the xPLStack to send out
//on the network.
 void XPLAction::execute()
{
	//return *responses_;
  //sendMsg(messagesToSend[i]);
  //for (vector<XPLMessage>::iterator rit = responses_->begin(); rit != responses_->end(); ++rit) {
      //XPLParser::Instance()->sendMsg(*rit);    
  //}
  XPLParser::Instance()->sendMsg(message);
  
}


// Compares action objects based on the attributes of their contained messages.
// @return true if equal.
bool XPLAction::equals(XPLAction* action)
{
	for(int i = 0; i < responses_->size(); i++)
	{
			
	}
}

void XPLAction::appendAction(pugi::xml_node* outputnode) {

    pugi::xml_node actionnode = outputnode->append_child("xplAction");
    actionnode.append_attribute("display_name") = "test";
    actionnode.append_attribute("executeOrder") = "001";
    actionnode.append_attribute("msg_type") = message.getMsgType().c_str();
    XPLAddress destinationAddress = message.getDestination();
    actionnode.append_attribute("msg_target") = (destinationAddress.vendor+"."+destinationAddress.device+"."+destinationAddress.instance).c_str();
    actionnode.append_attribute("msg_schema") = (message.getSchema().schema + "." + message.getSchema().type).c_str();
    
    for(int i = 0; i<message.getMembers().size(); i++)
    {
        pugi::xml_node paramnode = actionnode.append_child("xplActionParam");
        paramnode.append_attribute("expression") =  (message.getMembers()[i].member + "=" + message.getMembers()[i].value).c_str();
        
    }

 }
 
 
