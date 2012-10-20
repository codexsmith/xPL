#include "GlobalCondition.h"

#include "Determinator.h"
#include <string>
#include <iostream>
#include <string.h>
#include "Poco/NumberParser.h"

using namespace std;
using namespace Poco;

GlobalCondition::GlobalCondition ( string nameIn, string operatorIn, string valueIn )
{
    globalName =nameIn;
    //globalOperator = DeterminatorOperator::Equals;
    globalOperator = DeterminatorOperator ( operatorIn );
    globalValue = valueIn;
}


GlobalCondition::GlobalCondition ( pugi::xml_node condnode )
{

    bool failed = false;
    //attributes_ = new vector<XPLValuePair>();

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
        globalName = condnode.attribute ( "name" ).as_string();
    }
    else
    {
        failed = true;
    }
    if ( condnode.attribute ( "value" ) )
    {
        globalValue = condnode.attribute ( "value" ).as_string();
    }
    else
    {
        failed = true;
    }

    if ( condnode.attribute ( "operator" ) )
    {
        string globalOperatorStr = Determinator::unescape ( condnode.attribute ( "operator" ).as_string() );
        globalOperator = DeterminatorOperator ( globalOperatorStr );
    }
    else
    {
        failed = true;
    }
}

//match to see if the condition is true. We have to try to convert to a number if the operator requires it
//by the way, this is a mess. I don't like it, but I don't have any better ideas at the moment
bool GlobalCondition::match ( DeterminatorEnvironment* env )
{
    if ( !env->globals->hasGlobal ( globalName ) )
    {
        return false;
    }

    string currentVal = env->globals->getGlobal ( globalName );

    //first try to think of these both as strings.
    if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::Equals ) )
    {
        return ( globalValue == currentVal );
    }
    else if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::NotEquals ) )
    {
        return ! ( globalValue == currentVal );
    }
    else
    {
        //we'll have to try converting to numbers
        int curIntVal;
        int compIntVal;
        if ( ! ( NumberParser::tryParse ( currentVal,curIntVal ) &&  NumberParser::tryParse ( globalValue,compIntVal ) ) )
        {
            //can't convert to numbers
            cout << "can't parse numbers" << std::endl;
            return false;
        }

        //cout << "testing " << globalName << " (" << compIntVal << ") " << globalOperator.toString() << " " << curIntVal << std::endl;

        //now compare
        if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::LessThan ) )
        {
            return ( curIntVal < compIntVal );
        }
        else if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::GreaterThan ) )
        {
            return ( curIntVal > compIntVal );
        }
        else if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::LessThanOrEquals ) )
        {
            return ( curIntVal <= compIntVal );
        }
        else if ( globalOperator==DeterminatorOperator ( DeterminatorOperator::GreaterThanOrEquals ) )
        {
            return ( curIntVal >= compIntVal );
        }
    }

    return false;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool GlobalCondition::equals ( GlobalCondition* condition )
{
    return ( ( condition->globalName == globalName )
             && ( condition->globalOperator == globalOperator )
             && ( condition->globalValue  == globalValue )
             && ( condition->display_name == display_name ) );

}


void GlobalCondition::appendCondition ( pugi::xml_node* inputnode )
{

    pugi::xml_node condnode = inputnode->append_child ( "globalCondition" );

    condnode.append_attribute ( "display_name" ) = display_name.c_str();
    condnode.append_attribute ( "name" ) = globalName.c_str();
    condnode.append_attribute ( "operator" ) = globalOperator.toString().c_str();
    condnode.append_attribute ( "value" ) = globalValue.c_str();

}

