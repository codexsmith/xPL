#include "XPLHal.h"


#include <vector>
#include <string>
#include <iostream>

#include "XPLAction.h"
#include "XPLParser.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/String.h"
#include "XHCPServerConnection.h"

#include <xplMsg.h>

using namespace std;
using Poco::Net::ServerSocket;
using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::IPAddress;

XPLHal::XPLHal() {
    cout << "XPLHal Created\n";
    //Load XML Determinators from disk.
    ruleMgr.assign(new XPLRuleManager());
    dispatch.assign(new XHCPDispatcher(this));
    

    
    
    xplUDP::instance()->rxNotificationCenter.addObserver(Observer<XPLHal, MessageRxNotification>(*this,&XPLHal::HandleAllMessages));
    
    cout << "app: xplUDP created\n";
    //pDevice = xplDevice( "smgpoe", "xplhal", "1.2", true, true, myComms );
    pDevice.assign(new xplDevice( "smgpoe", "xplhal", "1.2", true, true, xplUDP::instance() ));
    if( NULL == pDevice )
    {
        cout << "error\n\n";
        return;
    }
    
    pDevice->rxNotificationCenter.addObserver(Observer<XPLHal, MessageRxNotification>(*this,&XPLHal::HandleDeviceMessages));
    
    pDevice->Init();

    startXHCP();
    

    
    
    
}
XPLHal::~XPLHal() {
     cout << "XPLHal Destroying\n";
    cout << "XPLHal Destroyed\n";
    srv->stop();

}

//messages that are local to us
void XPLHal::HandleDeviceMessages(MessageRxNotification* mNot) {
    cout << "got directed message: " << mNot->message->GetSchemaClass() << " " <<mNot->message->GetSchemaType() << "\n";
    mNot->release();
}

//messages for anyone, to be processed by the rules engine
void XPLHal::HandleAllMessages(MessageRxNotification* mNot) {
    cout << "got message: " << mNot->message->GetSchemaClass() << " " <<mNot->message->GetSchemaType() << "\n";
    ruleMgr->match(*(mNot->message));
    mNot->release();

}


void XPLHal::saveDeterminators(void) {
    cout << "trying to save determinators (hal)\n";
    ruleMgr->saveDeterminators();
}


void XPLHal::startXHCP()
{
    
    
    ServerSocket sckt(3865);
    
//     srv.assign(new TCPServer(new TCPServerConnectionFactoryImpl<XHCPServerConnection>(ruleMgr), sckt));
    cout << "dispactchmaster: " << dispatch << " pt " << dispatch.get() << "\n";
    srv.assign(new TCPServer(new XHCPServerConnectionFactory(this), sckt));
    //TCPServer srv(new XHCPServerConnectionFactory(), sckt);
    
    srv->start();
    cout << "XHCP server started (?) at " << sckt.address().toString() <<  "\n";
    
}


