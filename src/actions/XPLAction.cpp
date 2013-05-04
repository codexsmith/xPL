#include <vector>
#include <string>
#include <iostream>

#include "XPLAction.h"
#include "XPLParser.h"
#include <XplComms.h>
#include <XplUDP.h>

using namespace std;

//XPLActions are directly constructed their vector of messages.
//Should only be used in an appropriate factory.
XPLAction::XPLAction() :
    actlog ( Logger::get ( "rulemanager.determinator.xplaction" ) )
{
    actlog.setLevel("trace");
}

//A constructor that builds the XPLAction from XML, passed in from the Action node down.
XPLAction::XPLAction ( pugi::xml_node actionnode ) :
    actlog ( Logger::get ( "rulemanager.determinator.xplaction" ) )
{
    bool failed = false;
    string type_str;
    string source_str = "smgpoe-fixme.2";
    string target_str;
    string schemaClass_str;
    string schemaType_str;

    if ( actionnode.attribute ( "display_name" ) )
    {
        display_name = actionnode.attribute ( "display_name" ).as_string();
    }
    else
    {
        failed = true;
    }
    if ( actionnode.attribute ( "executeOrder" ) )
    {
        executeOrder = actionnode.attribute ( "executeOrder" ).as_int();
    }
    else
    {
        failed = true;
    }
    if ( actionnode.attribute ( "msg_type" ) )
    {
        type_str = actionnode.attribute ( "msg_type" ).as_string();
    }
    else
    {
        failed = true;
    }

    if ( actionnode.attribute ( "msg_target" ) )
    {
        target_str = actionnode.attribute ( "msg_target" ).as_string();
    }
    else
    {
        failed = true;
    }

    if ( actionnode.attribute ( "msg_schema" ) )
    {
        string schema = actionnode.attribute ( "msg_schema" ).as_string();
        int schemaSplit = schema.find ( "." );
        schemaClass_str = schema.substr ( 0, schemaSplit );
        schemaType_str = schema.substr ( schemaSplit+1, schema.length() - schemaSplit );
    }
    else
    {
        failed = true;
    }

    message = new XplMsg ( type_str, source_str, target_str, schemaClass_str, schemaType_str );

//     cout << "target: " << target_str << "\n";

    //pugi::xml_node outnode =actionnode.child("output");
    for ( pugi::xml_node_iterator ait = actionnode.begin(); ait != actionnode.end(); ++ait )
    {
        //cout << "m: " << ait->name() << ". \n";
        if ( string ( ait->name() ) =="xplActionParam" )
        {
            if ( ( *ait ).attribute ( "expression" ) )
            {
                string expr = ( *ait ).attribute ( "expression" ).as_string();
                int exprsplit = expr.find ( "=" );
                string member = expr.substr ( 0, exprsplit );
                string value = expr.substr ( exprsplit+1, expr.length() - exprsplit );

                //message.addMember(member, value);
                message->AddValue ( member,value );
            }
            else
            {
                failed = true;
            }
        }
    }
    //cout << "\t\tloaded " << message.getMembers().size() << " members\n";
    poco_debug ( actlog, "Loaded " + NumberFormatter::format ( message->GetNumMsgItems() ) + " members" );


}



//Method to execute the determinator.
//@return returns a vector of XPLMessages for the xPLStack to send out
//on the network.
void XPLAction::execute ( DeterminatorEnvironment* env )
{
    //XPLParser::instance().sendMsg(message); //FIXME
    XplUDP* comm = XplUDP::instance();
    comm->TxMsg ( *message );
    poco_information ( actlog, "Action \"" + display_name + "\" sending a message" );
    poco_trace ( actlog, message->GetRawData() );
}


// Compares action objects based on the attributes of their contained messages.
// @return true if equal.
bool XPLAction::equals ( XPLAction* action )
{
    if ( display_name!=action->display_name ) return false;
    if ( executeOrder!=action->executeOrder ) return false;
    if ( message!=action->message ) return false;


}

//Takes the current Action, and packs it up into XML, appending it all to outputnode.
void XPLAction::appendAction ( pugi::xml_node* outputnode )
{

    pugi::xml_node actionnode = outputnode->append_child ( "xplAction" );
    actionnode.append_attribute ( "display_name" ) = display_name.c_str();
    actionnode.append_attribute ( "executeOrder" ) = executeOrder;
    actionnode.append_attribute ( "msg_type" ) = message->GetType().substr ( 4 ).c_str(); //message->getMsgType().c_str();
    //XPLAddress destinationAddress = message->GetTarget(); //getDestination();
    actionnode.append_attribute ( "msg_target" ) = message->GetTarget().toString().c_str(); //(destinationAddress.vendor+"."+destinationAddress.device+"."+destinationAddress.instance).c_str();
    actionnode.append_attribute ( "msg_schema" ) = ( message->GetSchemaClass() + "." + message->GetSchemaType() ).c_str();

    for ( int i = 0; i<message->GetNumMsgItems(); i++ )
    {
        pugi::xml_node paramnode = actionnode.append_child ( "xplActionParam" );
        paramnode.append_attribute ( "expression" ) = ( message->GetMsgItem ( i )->GetName() + "=" + message->GetMsgItem ( i )->GetValue() ).c_str();

    }

}


