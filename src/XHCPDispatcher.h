#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Poco/SharedPtr.h"
#include "Poco/Net/SocketStream.h"

#include <sstream>
#include <iostream>

class XPLHal;

using Poco::Net::SocketStream;
using namespace Poco;
using namespace std;
class XPLRuleManager;

static string XHCPCRLF = "\r\n";

class XHCPDispatcher
{
public:

    XHCPDispatcher ( std::string );
    XHCPDispatcher ( XPLHal* halin );
    ~XHCPDispatcher() {};

    std::string delGlobal ( std::string , SocketStream& strm );
    std::string getGlobal ( std::string , SocketStream& strm );
    std::string listGlobals ( std::string , SocketStream& strm );
    std::string setGlobal ( std::string , SocketStream& strm );


    std::string listRules ( std::string  , SocketStream& strm );
    std::string listRuleGroups ( std::string , SocketStream& strm );
    std::string runRule ( std::string  , SocketStream& strm );
    std::string setRule ( std::string , SocketStream& strm );
    std::string delRule ( std::string  , SocketStream& strm );
    std::string getRule ( std::string  , SocketStream& strm );

    std::string capCommand ( std::string  , SocketStream& strm );
    std::string listDevices ( std::string aString , SocketStream& strm );
    std::string listSubs ( std::string aString , SocketStream& strm );
    std::string listOptions ( std::string  , SocketStream& strm );
    std::string quit ( std::string  , SocketStream& strm );
    std::string addEvent ( std::string  , SocketStream& strm );
    
private:
    std::string name;
    XPLHal* hal;
    SharedPtr< XPLRuleManager > ruleMgr;
};

#endif
