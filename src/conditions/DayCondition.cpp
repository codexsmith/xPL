#include "DayCondition.h"
#include "XPLCommon.h"
#include "DeterminatorEnvironment.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include "Determinator.h"

#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

using namespace std;


DayCondition::DayCondition() :
    timeLogger ( Logger::get ( "rulemanager.determinator.daycondition" ) )
{

}

DayCondition::DayCondition ( pugi::xml_node condnode ) :
    timeLogger ( Logger::get ( "rulemanager.determinator.daycondition" ) )
{

    poco_trace ( timeLogger, "conditional being parsed" );


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
    if ( condnode.attribute ( "dow" ) )
    {
        string dowval = condnode.attribute ( "dow" ).as_string();
        poco_trace ( timeLogger, "value from conditional is " + dowval );
        while ( dowval.length() > 0 )
        {
            if ( ( dowval[0] ) == '1' )
            {
                poco_trace ( timeLogger, "dow set true" );
                days.push_back ( true );
            }
            else
            {
                poco_trace ( timeLogger, "dow set false" );
                days.push_back ( false );
            }

            dowval = dowval.substr ( 1 );
        }

    }
    else
    {
        failed = true;
    }
    poco_trace ( timeLogger, "done" );

}

DayCondition::~DayCondition()
{

}


//Matches a message. Returns true when all of the CONDITION'S parameters have
//matching values in the passed in message.
bool DayCondition::match ( DeterminatorEnvironment* env )
{
    bool ret = false;

    DateTime envDT ( env->mtime );
    envDT.makeLocal ( Timezone::tzd() );
    int dow = envDT.dayOfWeek();
    poco_trace ( timeLogger, "current: "  + DateTimeFormatter::format ( envDT, "%e %b %Y  %H:%M" ) );
    if ( dow < days.size() )
    {
        ret = days[dow];
        if ( ret )
        {
            poco_trace ( timeLogger, "dow match: " + NumberFormatter::format ( dow ) + " is true" );
        }
        else
        {
            poco_trace ( timeLogger, "dow match: " + NumberFormatter::format ( dow ) + " is false" );
        }
    }
    else
    {
        poco_error ( timeLogger, "dow internal error" );
    }


    //cout << ret << "\n";
    return ret;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool DayCondition::equals ( DayCondition* condition )
{
    if ( ( condition->days == days )
            && ( condition ->display_name == display_name ) )
    {
        return true;
    }
    return false;
}

//Turns the DayCondition into a formatted XML string
//for serialization.
void DayCondition::appendCondition ( pugi::xml_node* inputnode )
{

    pugi::xml_node condnode = inputnode->append_child ( "dayCondition" );
    condnode.append_attribute ( "display_name" ) = display_name.c_str();

    string dowstr = "";
    vector<bool>::iterator bit;
    for ( bit = days.begin(); bit < days.end(); ++bit )
    {
        if ( *bit )
        {
            dowstr.append ( "1" );
        }
        else
        {
            dowstr.append ( "0" );
        }
    }
    condnode.append_attribute ( "dow" ) = dowstr.c_str();

}

