#include "TimeCondition.h"
#include "XPLCommon.h"
#include "DeterminatorEnvironment.h"

#include <string>
#include <iostream>
#include <sstream> 
#include <stdio.h>
#include <string.h>
#include "Determinator.h"
using namespace std;


TimeCondition::TimeCondition() {
    
}


TimeCondition::TimeCondition(pugi::xml_node condnode) {
    
    bool failed = false;
    //attributes_ = new vector<XPLValuePair>();
    
    if (condnode.attribute("display_name")) {
        display_name = condnode.attribute("display_name").as_string();
    } else {
        failed = true;
    }
    if (condnode.attribute("operator")) {
        toprator = Determinator::unescape(condnode.attribute("operator").as_string());
    } else {
        failed = true;
    }
    if (condnode.attribute("value")) {
        string timeVal = condnode.attribute("value").as_string();
        int splitc = timeVal.find_first_of(":");
        
        stringstream convert(timeVal.substr(0,splitc));
        convert>>thours;
        stringstream convert2(timeVal.substr(splitc+1));
        convert2>>tminutes;


        
    } else {
        failed = true;
    }
    //cout << "\t\tloaded " << attributes_.size() << " attributes\n";
}

TimeCondition::~TimeCondition()
{
 	
}


//Matches a message. Returns true when all of the CONDITION'S parameters have
//matching values in the passed in message.
bool TimeCondition::match(DeterminatorEnvironment* env)
{
    bool ret;
    //cout << "\t\ttime cond testing: time: " << env->mtime->tm_hour <<" : " <<  env->mtime->tm_min << " " << toprator <<" " << thours << ":" << tminutes << "  ";

    int etime = ( 60  * env->mtime->tm_hour ) + env->mtime->tm_min;
    int ttime = ( 60 * thours ) + tminutes;
    //cout << "\t\ttime cond testing: time: " << etime << " " << toprator <<" " << ttime << "  ";
    
    if ( !strcmp(toprator.c_str(), "=")) {
        ret =  (etime == ttime);
    } else if ( !strcmp(toprator.c_str(), "!=")) {
        ret =  (etime != ttime);
    } else if ( !strcmp(toprator.c_str(), ">")) {
        ret =  (etime > ttime);
    } else if ( !strcmp(toprator.c_str(), "<")) {
        ret =  (etime < ttime);
    } else if ( !strcmp(toprator.c_str(), ">=")) {
        ret =  (etime >= ttime);
    } else if ( !strcmp(toprator.c_str(), "<=")) {
        ret =  (etime <= ttime);
    } else {
        cout << "no idea what that operator is: " << toprator << "\n";
        return false;   
    }
    
    //cout << ret << "\n";
    return ret;
}

//Compares conditions based on the equivalencey of each of their member variables.
bool TimeCondition::equals(TimeCondition* condition)
{
    if ((condition->thours == thours)
        && (condition ->tminutes == tminutes)
      && (condition ->display_name == display_name)
	    && (condition -> toprator == toprator)) {
	      return true;
	  }
	  return false;
}

//Turns the TimeCondition into a formatted XML string
//for serialization.
void TimeCondition::appendCondition(pugi::xml_node* inputnode) {
    
    pugi::xml_node condnode = inputnode->append_child("timeCondition");
    
    condnode.append_attribute("display_name") = display_name.c_str();/*
    condnode.append_attribute("category") = "time"; //FIXME this isn't in the spec*/
    condnode.append_attribute("operator") = toprator.c_str();
    char str[7];
    snprintf (str, 6, "%02d:%02d", thours, tminutes);
    condnode.append_attribute("value") = str;
}

