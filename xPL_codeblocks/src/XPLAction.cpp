#include <vector>
#include <string>

#include "XPLAction.h"
#include "XPLMessage.h"
#include "XPLParser.h"

using namespace std;

//XPLActions are directly constructed their vector of messages.
//Should only be used in an appropriate factory.
XPLAction::XPLAction(vector<XPLMessage>* responses)
{
	responses_ = responses;
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
  for (vector<XPLMessage>::iterator rit = responses_->begin(); rit != responses_->end(); ++rit) {
      XPLParser::Instance()->sendMsg(*rit);    
  }
  
  
}


// Compares action objects based on the attributes of their contained messages.
// @return true if equal.
bool XPLAction::equals(XPLAction* action)
{
	for(int i = 0; i < responses_->size(); i++)
	{
			
	}
}

 
// Method to convert the action into an XML string.
// @preturn XML formatted string respresenting the action.
string XPLAction::printXML()
{
	string result = "";
	for(int i = 0; i<responses_->size(); i++)
	{
		XPLMessage message = responses_->at(i);
		result.append(tabLevel + "\n");
		result.append(tabLevel + "msg_type=");
		result.append(message.getMsgType() + "\n");
		result.append(tabLevel + "msg_target=");
		XPLAddress destinationAddress = message.getDestination();
		result.append(destinationAddress.vendor+"."+destinationAddress.device+"."+destinationAddress.instance+"\n");
		result.append(tabLevel + "msg_schema=");
		XPLSchema schema = message.getSchema();
		result.append(schema.schema + "." + schema.type);
		result.append("\n>\n");
		vector<XPLValuePair> members = message.getMembers();
		for(int i = 0; i<members.size(); i++)
		{
			result.append(tabLevel + "\t" + "<xplActionParam " + "\n"); 
			result.append(tabLevel + "\t\t" +	"expression=" + members[i].member + "=" + members[i].value);
			result.append("\n\t\t\t>\n");
		}

	}
	result.append(tabLevel+"</xplaction>");
	return result;
}