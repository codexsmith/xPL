#include "XHCPDispatcher.h"
#include "XPLRuleManager.h"
#include "Poco/String.h"
#include "XPLHal.h"



XHCPDispatcher::XHCPDispatcher ( std::string aName )
{
    name = aName;
}
XHCPDispatcher::XHCPDispatcher ( XPLHal* halin )
{

    hal = halin;
    ruleMgr = hal->ruleMgr;
}


std::string XHCPDispatcher::delGlobal ( std::string strIn , SocketStream& strm )
{
    int split = strIn.find_first_of ( " " );
    string arg = strIn.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret;
    cout << "arg is " << arg << "\n";
    hal->globals->deleteGlobal ( arg );
    return "233 Global deleted\n";
}
std::string XHCPDispatcher::getGlobal ( std::string strIn, SocketStream& strm )
{
    int split = strIn.find_first_of ( " " );
    string arg = strIn.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    cout << "arg is " << arg << "\n";
    map<string,string> globalcp = hal->globals->getGlobals();

    string ret = "";
    if ( globalcp.count ( arg ) )
    {
        ret += "291 Global value follows" + XHCPCRLF;
        ret += globalcp[arg] + XHCPCRLF;
        ret += "." + XHCPCRLF;
    }
    else
    {
        ret += "491 No such global" + XHCPCRLF;
    }
    return ret;

}
std::string XHCPDispatcher::listGlobals ( std::string strIn, SocketStream& strm )
{
    string ret = "231 List of globals follows" + XHCPCRLF;
    map<string,string> globalcp = hal->globals->getGlobals();


    map<string,string>::iterator git;
    for ( git = globalcp.begin(); git != globalcp.end(); ++git )
    {
        ret += git->first + "=" + git->second + XHCPCRLF;
    }
    ret += "." + XHCPCRLF;
    return ret;
}
std::string XHCPDispatcher::setGlobal ( std::string strIn, SocketStream& strm )
{

    int split = strIn.find_first_of ( " " );
    string arg0 = strIn.substr ( 0,split );
    if ( !arg0.length() || arg0.length() == strIn.length() ) return ( "232 Global value updated" + XHCPCRLF );
    string rest = strIn.substr ( split );
    trimInPlace ( rest );

    if ( !rest.length() ) return ( "232 Global value updated" + XHCPCRLF );
//         split = rest.find_first_of(" ");
//         string arg1= rest.substr(0,split);
//
    toUpperInPlace ( trimInPlace ( arg0 ) );
    hal->globals->setGlobal ( arg0,rest );

    return ( "232 Global value updated" + XHCPCRLF );
}

std::string XHCPDispatcher::runRule ( std::string aString , SocketStream& strm )
{

    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret;
    cout << "arg is " << arg <<  XHCPCRLF;
    bool exists = ruleMgr->runDeterminator ( arg );
    if ( exists )
    {
        return ( "203 Script/determinator executed" + XHCPCRLF );
    }
    return ( "410 No such script/determinator" + XHCPCRLF );
}


std::string XHCPDispatcher::setRule ( std::string aString , SocketStream& strm )
{

    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret;
    cout << "arg is " << arg << "\n";

    std::string theString = "338 Send rule, end with <CrLf>.<CrLf>" + XHCPCRLF;
    cout << theString;
    strm << theString;
    strm.flush();

    string line  = "";
    string detxml = "";

    while ( line != "." )
    {
        detxml += line;
        getline ( strm,line );
        trimInPlace ( line );

        //cout << "detline: " << line << "\n";

    }

    cout << "det: " << detxml << "::\n";
    Determinator* newdet = new Determinator ( detxml );
    if ( arg.length() )
    {
        cout << "modifing exisitng determinator " << arg << XHCPCRLF;
        newdet->setGUID ( arg );
    }

    ruleMgr->setDeterminator ( newdet->getGUID(), newdet );

    theString = "238 "  + newdet->getGUID() + XHCPCRLF;
    return theString;
}
std::string XHCPDispatcher::delRule ( std::string aString , SocketStream& strm )
{

    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret;
    cout << "arg is " << arg <<  XHCPCRLF;
    bool deleted = ruleMgr->removeDeterminator ( arg );
    if ( deleted )
    {
        return ( "214 Script/determinator successfully deleted" + XHCPCRLF );
    }
    return ( "410 No such script/determinator" + XHCPCRLF );
}

std::string XHCPDispatcher::listRules ( std::string aString , SocketStream& strm )
{
    std::string theString;


    theString += ( "237 List of Determinator Rules follows" + XHCPCRLF );

    //TODO deal with the thread-saftey issue here
    //TODO handle args like a group name or "{ALL}"
    ruleMgr->detLock.readLock();
    map<string, Determinator*>* dets = ruleMgr->determinators;
    for ( map<string, Determinator*>::iterator dit = dets->begin(); dit!=dets->end(); ++dit )
    {
        string detString;
        detString += dit->first + "\t";
        detString += dit->second->name + "\t";
        detString += dit->second->isEnabled() + "\t";
        theString += detString  + XHCPCRLF;
    }
    ruleMgr->detLock.unlock();
    theString += "." + XHCPCRLF;
    //String += "b\r\n";
    return theString;
}
std::string XHCPDispatcher::listRuleGroups ( std::string aString , SocketStream& strm )
{
    std::string theString;

    theString +=  "240 List of determinator groups follows" + XHCPCRLF;

    ruleMgr->detLock.readLock();
    map<string, Determinator*>* dets = ruleMgr->determinators;
    for ( map<string, Determinator*>::iterator dit = dets->begin(); dit!=dets->end(); ++dit )
    {
        if ( dit->second->isGroup )
        {
            theString += dit->first + "\t";
            theString += dit->second->groupName  + XHCPCRLF;
        }
    }
    ruleMgr->detLock.unlock();

    //TODO implement rule groups
    //theString += "4242\tFIXME group name\t\n";
    theString += "." + XHCPCRLF;
    return theString;
}

std::string XHCPDispatcher::getRule ( std::string aString , SocketStream& strm )
{
    string arg = trim ( aString );
    string ret;
    Determinator* det = ruleMgr->retrieveDeterminator ( arg );
    if ( det )
    {
        ret += "210 Requested script/rule follows" + XHCPCRLF;
        ret += det->printXML();
        ret += XHCPCRLF;
        ret += "." + XHCPCRLF;
    }
    else
    {
        ret += "410 No such script/rule" + XHCPCRLF;
    }
    return ret;

}
std::string XHCPDispatcher::capCommand ( std::string aString , SocketStream& strm )
{
    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );

    std::string theString = "236 --010L0" + XHCPCRLF;
    if ( arg == "SCRIPTING" )
    {
        theString = "241 --010L0" + XHCPCRLF;
        //"B<tab>BeanShell<tab>1.54<tab>bsh<tab>http://www.beanshell.org"
        theString += "." + XHCPCRLF;
    }

    return theString;
}
std::string XHCPDispatcher::listDevices ( std::string aString , SocketStream& strm )
{
    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret="";
    cout << "arg is " << arg << "\n";

    ret += "216 List of XPL devices follows" + XHCPCRLF;
    if ( arg == "AWAITINGCONFIG" )
    {

    }
    else if ( arg == "CONFIGURED" )
    {

    }
    else if ( arg == "MISSINGCONFIG" )
    {

    }
    else
    {

    }
    ret += "." + XHCPCRLF;
    return ret;
}

std::string XHCPDispatcher::listSubs ( std::string aString , SocketStream& strm )
{
    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    toUpperInPlace ( trimInPlace ( arg ) );
    string ret;
    cout << "arg is " << arg << XHCPCRLF;

    ret += "224 List of subs follows" + XHCPCRLF;

    ret += ".\r\n";
    return ret;
}



std::string XHCPDispatcher::listOptions ( std::string aString , SocketStream& strm )
{
    int split = aString.find_first_of ( " " );
    string arg = aString.substr ( 0,split );
    trimInPlace ( arg );
    string ret = "";

    cout << "arg is " << arg << "\n";
    //ret += ".\r\n";
    ret = "405 No such setting" + XHCPCRLF;
    return ret;
}
std::string XHCPDispatcher::quit ( std::string aString , SocketStream& strm )
{
    string ret = "221 Closing transmission channel - goodbye." + XHCPCRLF;
    return ret;
}

