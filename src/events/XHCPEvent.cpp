#include "XPLCommon.h"
#include "XHCPEvent.h"

#include <sstream>
#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/DateTimeParser.h>
#include <Poco/NumberFormatter.h>

using namespace std;
using namespace Poco;

XHCPEvent::XHCPEvent ( string description )
{
    Logger& xhcplog = Logger::get ( "XHCPEvent" );
    xhcplog.setLevel("trace");
    xhcplog.error("event descr: " + description + "::s");
    std::stringstream iss(description);
    
    while(iss.good())
    {
        std::string singleLine;
        getline(iss,singleLine);
        xhcplog.trace("s descr: " + singleLine + "::");
        int split = singleLine.find_first_of('=');
        if (split == string::npos){
            xhcplog.error("event line doesn't make sense: \"" + singleLine + "\"");
            continue;
        }
        string lhs = singleLine.substr(0,split);
        string rhs = singleLine.substr(split+1,string::npos);
        xhcplog.trace("event line:  " + lhs + " 1=1 " + rhs );
        try {
            if(lhs=="dow")
            {
                if (rhs.length()<7) continue;
                for (int i = 0; i<7; i++){
                    daysofweek_[i] = (rhs[i]=='1');
                }
            } else if(lhs=="starttime")
            {
                if (rhs[2] == ':'){
                    startTimeMins_ = (NumberParser::parse(rhs.substr(0,2)) * 60) + NumberParser::parse(rhs.substr(3,2));
                    xhcplog.trace("start " + NumberFormatter::format(startTimeMins_) + "|" + rhs.substr(0,1));
                }
            } else if(lhs=="endtime")
            {
                if (rhs[2] == ':'){
                    endTimeMins_ = (NumberParser::parse(rhs.substr(0,2)) * 60) + NumberParser::parse(rhs.substr(3,2));
                    xhcplog.trace("end " + NumberFormatter::format(endTimeMins_));
                }
            } else if(lhs=="interval")
            {
                interval_ = NumberParser::parse(rhs);
            } else if(lhs=="params")
            {
                params_ = rhs;
            } else if(lhs=="rand")
            {
                randomizeMins_ = NumberParser::parse(rhs);
            } else if(lhs=="subname")
            {
                subname_ = rhs;
            } else if(lhs=="tag")
            {
                tag_ = rhs;
            } else if(lhs=="date")
            {
                int tdiff = 0;
                date_ = DateTimeParser::parse("%d/%b/%Y %H:%M", rhs, tdiff);
            }
        } catch (SyntaxException) {
          xhcplog.warning("can't parse: " + singleLine );
          continue;
        }
        
    }

}

string XHCPEvent::getDescription() 
{
    //     <tag><tab><subname><tab><params><tab><starttime><tab><endtime><tab><dow><tab><runtime><tab>
    string ret = tag_ + "\t";

    ret += subname_ + "\t";
    ret += params_ + "\t";
    ret += NumberFormatter::format0(startTimeMins_/60,2) + ":" + NumberFormatter::format0(startTimeMins_%60,2) + "\t";
    ret += NumberFormatter::format0(endTimeMins_/60,2) + ":" + NumberFormatter::format0(endTimeMins_%60,2) + "\t";
    for (int i=0; i<7; i++) {
        if(daysofweek_[i]){
            ret += "1";
        } else {
            ret += "0";
        }
    }
    ret += "\t";
    ret += "11:30TOFOFIXME";
    
    return ret;

}

string XHCPEvent::getFullText()
{
//     222 Event information follows
//     422 No such event
}


Poco::Timestamp XHCPEvent::getNextTimeOrNull() 
{
    return date_.timestamp();
    
}
