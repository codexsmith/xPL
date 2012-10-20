
#ifndef XHCPSERVERCONNECTION_H
#define XHCPSERVERCONNECTION_H


#include "XHCPDispatcher.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Thread.h"
#include <iostream>
#include <map>


using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;



using namespace std;

typedef std::string ( XHCPDispatcher::*pt2Member ) ( std::string, SocketStream& );

class XHCPServerConnection: public TCPServerConnection
{
public:
    XHCPServerConnection ( const StreamSocket& s ,XPLHal* halin );
    void run();
    XPLHal* hal;
    std::map<std::string,pt2Member> theMap;
};

class XHCPServerConnectionFactory: public TCPServerConnectionFactory
// A factory for TimeServerConnection.
{
public:
    XPLHal* hal;
    XHCPServerConnectionFactory ( XPLHal* halin )
    {
        hal = ( halin );
    }
    TCPServerConnection* createConnection ( const StreamSocket& socket )
    {
        return new XHCPServerConnection ( socket, hal );
    }

private:
    std::string _format;

};


#endif
