#include "GlobalChangedCondition.h"

#include "Determinator.h"
#include <string>
#include <iostream>
#include <string.h>
#include "Poco/NumberParser.h"

using namespace std;
using namespace Poco;

GlobalChangedCondition::GlobalChangedCondition ( string nameIn )
{
    globalName =nameIn;
}


GlobalChangedCondition::GlobalChangedCondition ( pugi::xml_node condnode )
{

    bool failed = false;

    if ( condnode.attribute ( "display_name" ) )
    {
        display_name = condnode.attribute ( "display_name" ).as_string();
    }
    else
    {
        failed = true;
    }
    if ( condnode.attribute ( "name" ) )
    {
        globalName = GlobalManager::cleanGlobalName ( condnode.attribute ( "name" ).as_string() );
    }
    else
    {
        failed = true;
    }

}

//match to see if the condition is true. We have to try to convert to a number if the operator requires it
//by the way, this is a mess. I don't like it, but I don't have any better ideas at the moment
bool GlobalChangedCondition::match ( DeterminatorEnvironment* env )
{
    if ( ( env->envType == DeterminatorEnvironment::globalChanged )
            && ( env->globalName == globalName ) )
    {
        return true;
    }
    return false;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool GlobalChangedCondition::equals ( GlobalChangedCondition* condition )
{
    return ( ( condition->globalName == globalName )
             && ( condition->display_name == display_name ) );

}


void GlobalChangedCondition::appendCondition ( pugi::xml_node* inputnode )
{

    pugi::xml_node condnode = inputnode->append_child ( "globalChanged" );

    condnode.append_attribute ( "display_name" ) = display_name.c_str();
    condnode.append_attribute ( "name" ) = globalName.c_str();

}

