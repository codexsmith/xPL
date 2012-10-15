
#include "DeterminatorEnvironment.h"
#include "XPLCondition.h"
#include "XPLCommon.h"
#include "XPLHal.h"
#include "xplMsgItem.h"
#include "../../../src/heeks/skeleton/prim.h"
#include "Poco/RegularExpression.h"
#include "Poco/Exception.h"
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include "Poco/Timezone.h"
#include "Poco/NumberParser.h"



#include <string>
#include <iostream>

using namespace std;

using Poco::RegularExpression;
using Poco::RegularExpressionException;



DeterminatorEnvironment::DeterminatorEnvironment() {
  message = NULL;
  time_t stime = time(0);
  globals = XPLHal::instance().globals;
  envType = none;
}
DeterminatorEnvironment::DeterminatorEnvironment( AutoPtr< xplMsg > messagein) {
  message = messagein;
  time_t stime = time(0);
  globals = (XPLHal::instance()).globals;
  envType = xPLMessage;
}
DeterminatorEnvironment::DeterminatorEnvironment( string globalNameIn) {
    globalName = globalNameIn;
    time_t stime = time(0);
    globals = (XPLHal::instance()).globals;
    envType = globalChanged;
}

string DeterminatorEnvironment::replaceSingleValue(const string inputin) {
    string input = inputin.substr(1,inputin.length()-2);
    DateTime envdt;
    envdt.makeLocal(Timezone::tzd());
    
    if(input == "SYS::TIMESTAMP") {
        return DateTimeFormatter::format(envdt, "%Y%m%d%H%M%S");
    } else if(input == "SYS::DATE") {
        return DateTimeFormatter::format(envdt, "%d/%m/%Y");
    } else if(input == "SYS::DATE_UK") {
        return DateTimeFormatter::format(envdt, "%d/%m/%Y");
    } else if(input == "SYS::DATE_US") {
        return DateTimeFormatter::format(envdt, "%m/%d/%Y");
    } else if(input == "SYS::DATE_YMD") {
        return DateTimeFormatter::format(envdt, "%Y%m%d");
    } else if(input == "SYS::TIME") {
        return DateTimeFormatter::format(envdt, "%H%M%S");
    } else if(input == "SYS::DAY") {
        return DateTimeFormatter::format(envdt, "%e");
    } else if(input == "SYS::MONTH") {
        return DateTimeFormatter::format(envdt, "%n");
    } else if(input == "SYS::YEAR") {
        return DateTimeFormatter::format(envdt, "%Y");
    } else if(input == "SYS::HOUR") {
        return DateTimeFormatter::format(envdt, "%M");
    } else if(input == "SYS::SECOND") {
        return DateTimeFormatter::format(envdt, "%S");
    } else if(input == "SYS::") {
        return DateTimeFormatter::format(envdt, "%Y%m%d%H%M%S");
    } else if(input.substr(0,5)=="XPL::") {
        if(envType == xPLMessage){
            Logger &mylog = Logger::get("valuereplacement");
            string elementindex = input.substr(5);
            int split = elementindex.find_first_of(':');
            if(split != string::npos && (split + 1 < elementindex.size())){
                string name = elementindex.substr(0,split);
                string indexstr = elementindex.substr(split);
                mylog.trace("i: " + name + " : " + elementindex.substr(split));
                int index = 0;
                NumberParser::tryParse(indexstr,index);
                const xplMsgItem* item = message->GetMsgItem(name);
                if(item == NULL) return "";
                string got = item->GetValue(index);
                return got;
            } else {
//                 for (int j=0; j< message->GetNumMsgItems(); j++) {
//                     mylog.debug("msgitem: " + NumberFormatter::format(j) + " = " + message->GetMsgItem(j)->GetName());
//                 }
                const xplMsgItem* item = message->GetMsgItem(elementindex);
                if(item == NULL) return "";
                string got = item->GetValue(0);
                return got;
            }
            return "";    
        }
        return "";
    } else if ((input.find_first_of("++") == input.length()-2) || (input.find_first_of("--") == input.length()-2)){ //check for "globalname++" and "globalname--"
            Logger &mylog = Logger::get("valuereplacement");
        string globalName = GlobalManager::cleanGlobalName(input.substr(0,input.length()-2));
        mylog.trace("globalname = " + globalName);
        string toRet = globals->getGlobal(globalName);
        int currNumber;
        if (globals->hasGlobal(globalName) && NumberParser::tryParse(toRet,currNumber)) {
            if((input.find_first_of("++") == input.length()-2)){
                //mylog.debug("global inc " + NumberFormatter::format(currNumber+1));
                //XPLHal::instance().globals->setGlobal(globalName, NumberFormatter::format(currNumber+1));
                toRet = NumberFormatter::format(currNumber+1);
            }else {
                //mylog.debug("global dec " + NumberFormatter::format(currNumber-1));
                //XPLHal::instance().globals->setGlobal(globalName, NumberFormatter::format(currNumber-1));
                toRet = NumberFormatter::format(currNumber-1);
            }
        }
        return toRet;
    } else if(globals->hasGlobal(GlobalManager::cleanGlobalName(input))){
        return globals->getGlobal(GlobalManager::cleanGlobalName(input));
    }
    return "";
}

string DeterminatorEnvironment::handleValueReplacement(const string input) {
    string output = input;
    Logger &mylog = Logger::get("valuereplacement");
    mylog.trace("input: " + input);
    
    vector< pair <int,int > > toReplace;
    int lastopen = -1;
    for (int pos = 0; pos < input.length(); pos++) {
        if (input[pos] == '{') {
            lastopen = pos;
        } else if ((input[pos] == '}') && (lastopen != -1))  {
            toReplace.push_back(pair<int,int>(lastopen, pos-lastopen+1));
            lastopen = -1;
        }
    }
    
    if(toReplace.size()) {
        mylog.trace("count: " + NumberFormatter::format(toReplace.size()));
        for(vector< pair <int,int > >::iterator it = toReplace.end()-1; it >= toReplace.begin(); --it) {
            mylog.trace("token: " +  NumberFormatter::format(it->first) + " " + NumberFormatter::format(it->second) + "  " + input.substr(it->first, it->second));
            output.replace(it->first, it->second, replaceSingleValue(input.substr(it->first, it->second)) );
        }
        mylog.trace("output: " + output);
    }
    return output;
    
}
