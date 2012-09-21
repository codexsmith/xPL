
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

typedef std::string (XHCPDispatcher::*pt2Member)(std::string);

class XHCPServerConnection: public TCPServerConnection
{
public:
    XHCPServerConnection ( const StreamSocket& s ,SharedPtr<XHCPDispatcher> dispatchin );
    void run();
    SharedPtr<XHCPDispatcher> dispatch;
    std::map<std::string,pt2Member> theMap;
};

class XHCPServerConnectionFactory: public TCPServerConnectionFactory
// A factory for TimeServerConnection.
{
public:
    SharedPtr<XHCPDispatcher> dispatch;
    XHCPServerConnectionFactory ( SharedPtr<XHCPDispatcher> dispatchin ) 
    {
        dispatch = dispatchin;
        cout << "fac created\n";
    }

    TCPServerConnection* createConnection(const StreamSocket& socket) {
        cout << "create called\n";
        return new XHCPServerConnection ( socket, dispatch);
    }

private:
    std::string _format;
    
};


#endif
