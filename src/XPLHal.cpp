#include "XPLHal.h"


#include <vector>
#include <string>
#include <iostream>


#include "XPLParser.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/String.h"
#include "XHCPServerConnection.h"
#include <Poco/Path.h>
#include <xplMsg.h>

using namespace std;
using Poco::Net::ServerSocket;
using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::IPAddress;

XPLHal::XPLHal() :
hallog(Logger::get("xplhal")) {
    
    globals = SharedPtr<GlobalManager>(new GlobalManager());
   
    
    
    
}

void XPLHal::start() {
    
    //Load XML Determinators from disk.
    ruleMgr.assign(new XPLRuleManager());
    dispatch.assign(new XHCPDispatcher(this));
    
    
    globals->loadGlobals();
    
    xplUDP::instance()->rxNotificationCenter.addObserver(Observer<XPLHal, MessageRxNotification>(*this,&XPLHal::HandleAllMessages));
    
    poco_debug(hallog, "xplUDP created");
    //pDevice = xplDevice( "smgpoe", "xplhal", "1.2", true, true, myComms );
    pDevice.assign(new xplDevice( "smgpoe", "xplhal", "1.2", true, true, xplUDP::instance() ));
    if( NULL == pDevice )
    {
        poco_error(hallog, "no device");
        return;
    }
    
    pDevice->rxNotificationCenter.addObserver(Observer<XPLHal, MessageRxNotification>(*this,&XPLHal::HandleDeviceMessages));
    
    pDevice->Init();
    
    startXHCP();
    
    
}

XPLHal::~XPLHal() {
    poco_debug(hallog, "destroying xplHal");
    srv->stop();
    globals->saveGlobals();

}

namespace{
    //static Poco::SingletonHolder<XPLHal> sh;
    static XPLHal* halp;
}

XPLHal& XPLHal::instance() {
    //return *sh.get();
//     cout << "halp : " << halp << "\n";
    return *halp;
}


XPLHal& XPLHal::createInstance() {
//     cout << "creating instance\n";
    halp = new XPLHal();
    
    return *halp;
}
void XPLHal::deleteInstance() {
    delete halp;
}

//messages that are local to us
void XPLHal::HandleDeviceMessages(MessageRxNotification* mNot) {
    poco_debug(hallog, "got directed message: " + mNot->message->GetSchemaClass() + " " + mNot->message->GetSchemaType());
    mNot->release();
}

//messages for anyone, to be processed by the rules engine
void XPLHal::HandleAllMessages(MessageRxNotification* mNot) {
    poco_debug(hallog, " " + mNot->message->GetSchemaClass() + " " + mNot->message->GetSchemaType());
    
    //create an environment for this message
    DeterminatorEnvironment env((mNot->message));
    ruleMgr->match(env);
    mNot->release();

}


void XPLHal::saveDeterminators(void) {
    poco_debug(hallog, "trying to save determinators");
    ruleMgr->saveDeterminators();
}


void XPLHal::startXHCP()
{
    
    
    ServerSocket sckt(3865);
    
//     srv.assign(new TCPServer(new TCPServerConnectionFactoryImpl<XHCPServerConnection>(ruleMgr), sckt));
//     cout << "dispactchmaster: " << dispatch << " pt " << dispatch.get() << "\n";
    poco_debug(hallog, "XHCP connection listerner set up");
    srv.assign(new TCPServer(new XHCPServerConnectionFactory(this), sckt));
    //TCPServer srv(new XHCPServerConnectionFactory(), sckt);
    
    srv->start();
    poco_debug(hallog, "XHCP server started at " + sckt.address().toString());
//     cout << "XHCP server started (?) at " << sckt.address().toString() <<  "\n";
    
}


Path XPLHal::getConfigFileLocation() {
    Poco::Path p(Poco::Path::home());
    p.pushDirectory(".xPL");
    p.pushDirectory("xplhallite");
    return p;
}


