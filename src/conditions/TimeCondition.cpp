#include "TimeCondition.h"
#include "XPLCommon.h"
#include "DeterminatorEnvironment.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
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


// static maps to go from condition type to condition string and back
const TimeCondition::categoryMapT::value_type rawData[] =
{
    TimeCondition::categoryMapT::value_type ( "time", TimeCondition::cat_time ),
    TimeCondition::categoryMapT::value_type ( "date", TimeCondition::cat_date ),
    TimeCondition::categoryMapT::value_type ( "day", TimeCondition::cat_day ),
    TimeCondition::categoryMapT::value_type ( "month", TimeCondition::cat_month ),
    TimeCondition::categoryMapT::value_type ( "year", TimeCondition::cat_year ),
};
const int numElems = sizeof rawData / sizeof rawData[0];
TimeCondition::categoryMapT categoryMap ( rawData, rawData + numElems );

const TimeCondition::categoryMapTr::value_type rawDatar[] =
{
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_time, "time" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_date, "date" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_day, "day" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_month, "month" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_year, "year" ),
};
const int numElemsr = sizeof rawDatar / sizeof rawDatar[0];
TimeCondition::categoryMapTr categoryMapReverse ( rawDatar, rawDatar + numElemsr );


//a map for the category value formats
const TimeCondition::categoryFormatMapT::value_type rawDataf[] =
{
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_time, "%H:%M" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_date, "%e %b %Y" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_day, "%e" ),
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_month, "%B" ), //FIXME: this really rubs me the wrong way - we should be using "Mar" not "March" to match date category
    TimeCondition::categoryMapTr::value_type ( TimeCondition::cat_year, "%Y" ),
};
const int numElemsf = sizeof rawDataf / sizeof rawDataf[0];
TimeCondition::categoryFormatMapT categoryFormatMap ( rawDataf, rawDataf + numElemsf );



TimeCondition::TimeCondition() :
    timeLogger ( Logger::get ( "rulemanager.determinator.timecondition" ) )
{

}

TimeCondition::TimeCondition ( pugi::xml_node condnode ) :
    timeLogger ( Logger::get ( "rulemanager.determinator.timecondition" ) )
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
    if ( condnode.attribute ( "operator" ) )
    {
        toprator = Determinator::unescape ( condnode.attribute ( "operator" ).as_string() );
    }
    else
    {
        failed = true;
    }

    tcategory=TimeCondition::cat_time;
    if ( condnode.attribute ( "category" ) )
    {
        string categoryString = Determinator::unescape ( condnode.attribute ( "category" ).as_string() );
        if ( categoryMap.count ( categoryString ) )
        {
            tcategory=categoryMap[categoryString];
        }
    }

    if ( condnode.attribute ( "value" ) )
    {
        string timeVal = condnode.attribute ( "value" ).as_string();
        poco_trace ( timeLogger, "value from conditional is " + timeVal );

        poco_trace ( timeLogger, "format st: " + categoryFormatMap[tcategory] );
        try
        {

            int tzone;
            DateTimeParser::parse ( categoryFormatMap[tcategory] , timeVal, tValue,tzone );
            poco_trace ( timeLogger, "condition: " + categoryMapReverse[tcategory] + " - " + DateTimeFormatter::format ( tValue, categoryFormatMap[tcategory] ) );

        }
        catch ( SyntaxException e )
        {
            poco_notice ( timeLogger, "could not parse time/date" );
        }
    }
    else
    {
        failed = true;
    }
    poco_trace ( timeLogger, "done" );
    //cout << "\t\tloaded " << attributes_.size() << " attributes\n";
}

TimeCondition::~TimeCondition()
{

}


//Matches a message. Returns true when all of the CONDITION'S parameters have
//matching values in the passed in message.
bool TimeCondition::match ( DeterminatorEnvironment* env )
{
    bool ret;
    //cout << "\t\ttime cond testing: time: " << env->mtime->tm_hour <<" : " <<  env->mtime->tm_min << " " << toprator <<" " << thours << ":" << tminutes << "  ";
    //Timestamp etime(env->mtime);
    DateTime ourDT = tValue;
    //first we get a copy of the environment's datetime.
    DateTime envDT ( env->mtime );

    //then we make a new datetime, using the parts that we want to compare from out datetime, and the parts we don't care about from envDT
    if ( tcategory == TimeCondition::cat_time )
    {
        ourDT = DateTime ( envDT.year(), envDT.month(), envDT.day(), ourDT.hour(), ourDT.minute() );
    }
    else if ( tcategory == TimeCondition::cat_date )
    {
        ourDT = DateTime ( ourDT.year(), ourDT.month(), ourDT.day(), envDT.hour(), envDT.minute() );
    }
    else if ( tcategory == TimeCondition::cat_day )
    {
        ourDT = DateTime ( envDT.year(), envDT.month(), ourDT.day(), envDT.hour(), envDT.minute() );
    }
    else if ( tcategory == TimeCondition::cat_month )
    {
        ourDT = DateTime ( envDT.year(), ourDT.month(), envDT.day(), envDT.hour(), envDT.minute() );
    }
    else if ( tcategory == TimeCondition::cat_year )
    {
        ourDT = DateTime ( ourDT.year(), envDT.month(), envDT.day(), envDT.hour(), envDT.minute() );
    }
    else
    {
        return false;
    }

    Timestamp etime = env->mtime;
    Timestamp ttime = ourDT.timestamp();

    //int etime = ( 60  * env->mtime->tm_hour ) + env->mtime->tm_min;
    //int ttime = ( 60 * thours ) + tminutes;
    //cout << "\t\ttime cond testing: time: " << etime << " " << toprator <<" " << ttime << "  ";

    if ( !strcmp ( toprator.c_str(), "=" ) )
    {
        ret = ( etime == ttime );
    }
    else if ( !strcmp ( toprator.c_str(), "!=" ) )
    {
        ret = ( etime != ttime );
    }
    else if ( !strcmp ( toprator.c_str(), ">" ) )
    {
        ret = ( etime > ttime );
    }
    else if ( !strcmp ( toprator.c_str(), "<" ) )
    {
        ret = ( etime < ttime );
    }
    else if ( !strcmp ( toprator.c_str(), ">=" ) )
    {
        ret = ( etime >= ttime );
    }
    else if ( !strcmp ( toprator.c_str(), "<=" ) )
    {
        ret = ( etime <= ttime );
    }
    else
    {
        cout << "no idea what that operator is: " << toprator << "\n";
        poco_error ( timeLogger, "no idea what this operator is: " + toprator );
        return false;
    }


    poco_trace ( timeLogger, "Category: " + categoryMapReverse[tcategory] );
    poco_trace ( timeLogger, "Comparing:" );
    poco_trace ( timeLogger, "current: "  + DateTimeFormatter::format ( envDT, "%e %b %Y  %H:%M" ) );

    if ( ret )
        poco_trace ( timeLogger, "is " + toprator );
    else
        poco_trace ( timeLogger, "is not " + toprator );
    poco_trace ( timeLogger, "compare: " +  DateTimeFormatter::format ( ourDT, "%e %b %Y  %H:%M" ) );

    poco_trace ( timeLogger, "" );

    //cout << ret << "\n";
    return ret;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool TimeCondition::equals ( TimeCondition* condition )
{
    if ( ( condition->thours == thours )
            && ( condition ->tminutes == tminutes )
            && ( condition ->display_name == display_name )
            && ( condition -> toprator == toprator ) )
    {
        return true;
    }
    return false;
}

//Turns the TimeCondition into a formatted XML string
//for serialization.
void TimeCondition::appendCondition ( pugi::xml_node* inputnode )
{

    pugi::xml_node condnode = inputnode->append_child ( "timeCondition" );
    condnode.append_attribute ( "display_name" ) = display_name.c_str();

    condnode.append_attribute ( "category" ) = categoryMapReverse[tcategory].c_str();
    condnode.append_attribute ( "operator" ) = toprator.c_str();

    //format the value as needed as per the category
    string formattedValue = DateTimeFormatter::format ( tValue, categoryFormatMap[tcategory] );
    condnode.append_attribute ( "value" ) = formattedValue.c_str();

}

