#include <vector>
#include <string>
#include <iostream>

#include "LogAction.h"
#include <syslog.h>

using namespace std;



LogAction::LogAction ( string namein, string messagein ) :
    actlog ( Logger::get ( "rulemanager.determinator.logaction" ) )
{
    display_name = namein;
    messageText = messagein;
}


LogAction::LogAction ( pugi::xml_node actionnode ) :
    actlog ( Logger::get ( "rulemanager.determinator.logaction" ) )
{

    bool failed = false;

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
    if ( actionnode.attribute ( "logText" ) )
    {
        messageText = ( actionnode.attribute ( "logText" ).as_string() );
    }
    else
    {
        failed = true;
    }
}

LogAction::~LogAction()
{
    //cout << "deleting logaction: " << this << "\n";
}

void LogAction::execute ( DeterminatorEnvironment* env )
{
    //syslog(LOG_INFO, messageText.c_str());
    poco_warning ( actlog, env->handleValueReplacement ( messageText ) );
}


// Compares action objects based on the attributes of their contained messages.
// @return true if equal.
bool LogAction::equals ( LogAction* other )
{
    if ( ( messageText == other->messageText )
            && ( executeOrder == other->executeOrder )
            && ( display_name == other->display_name ) )
    {
        return true;
    }
    return false;
}

void LogAction::appendAction ( pugi::xml_node* outputnode )
{

    pugi::xml_node actionnode = outputnode->append_child ( "logAction" );
    actionnode.append_attribute ( "display_name" ) = display_name.c_str();
    actionnode.append_attribute ( "executeOrder" ) = executeOrder;
    actionnode.append_attribute ( "logText" ) = messageText.c_str();

}

