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
#include <Poco/Net/NetException.h>
#include <XplMsg.h>

using namespace std;
using Poco::Net::ServerSocket;
using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::IPAddress;
using namespace Poco::Net;

XPLHal::XPLHal() :
    hallog ( Logger::get ( "xplhal" ) )
{

    globals = SharedPtr<GlobalManager> ( new GlobalManager() );




}

void XPLHal::start()
{

    //Load XML Determinators from disk.
    ruleMgr.assign ( new XPLRuleManager() );
    dispatch.assign ( new XHCPDispatcher ( this ) );


    globals->loadGlobals();

    XplUDP::instance()->rxNotificationCenter.addObserver ( Observer<XPLHal, MessageRxNotification> ( *this,&XPLHal::HandleAllMessages ) );

    poco_debug ( hallog, "XplUDP created" );
    //pDevice = XplDevice( "smgpoe", "xplhal", "1.2", true, true, myComms );
    pDevice.assign ( new XplDevice ( "smgpoe", "xplhal", "1.2", true, XplUDP::instance() ) );
    if ( NULL == pDevice )
    {
        poco_error ( hallog, "no device" );
        return;
    }

    pDevice->rxNotificationCenter.addObserver ( Observer<XPLHal, MessageRxNotification> ( *this,&XPLHal::HandleDeviceMessages ) );

    save_adapter_ = new RunnableAdapter<XPLHal> ( *this,&XPLHal::SaveStateLoop );
    save_thread_.setName ( "Save state loop" );
    save_thread_.start ( *save_adapter_ );
    
    pDevice->Init();

    startXHCP();


}

XPLHal::~XPLHal()
{
    poco_debug ( hallog, "destroying xplHal" );
    srv->stop();
    SaveState();
    save_state_request_queue_.enqueueNotification(new QuitNotification());
    save_thread_.join();

}

namespace
{
//static Poco::SingletonHolder<XPLHal> sh;
static XPLHal* halp;
}

XPLHal& XPLHal::instance()
{
    //return *sh.get();
//     cout << "halp : " << halp << "\n";
    return *halp;
}


XPLHal& XPLHal::createInstance()
{
//     cout << "creating instance\n";
    halp = new XPLHal();

    return *halp;
}
void XPLHal::deleteInstance()
{
    delete halp;
}

//messages that are local to us
void XPLHal::HandleDeviceMessages ( MessageRxNotification* mNot )
{
    poco_debug ( hallog, "got directed message: " + mNot->message->GetSchemaClass() + " " + mNot->message->GetSchemaType() );
    mNot->release();
}

//messages for anyone, to be processed by the rules engine
void XPLHal::HandleAllMessages ( MessageRxNotification* mNot )
{
    poco_debug ( hallog, " " + mNot->message->GetSchemaClass() + " " + mNot->message->GetSchemaType() );

    //create an environment for this message
    DeterminatorEnvironment env ( ( mNot->message ) );
    ruleMgr->match ( env );
    mNot->release();

}

void XPLHal::SaveStateLoop()
{
    while(1){
        
        AutoPtr<Notification> notification = save_state_request_queue_.waitDequeueNotification();
        
        SaveStateNotification::Ptr pSaveNf = notification.cast<SaveStateNotification>();
        if ( pSaveNf )
        {
            poco_trace ( hallog, "saving changes to disk" );
            if(pSaveNf->save_globals_)
                globals->saveGlobals();
            if(pSaveNf->save_determinators_)
                saveDeterminators();
            continue;
        }

        QuitNotification::Ptr pQuitNf = notification.cast<QuitNotification>();
        if ( pQuitNf )
        {
            poco_trace ( hallog, "got quit notification, exiting work thread" );
            return;
        }
    }
}


void XPLHal::SaveState()
{
    save_state_request_queue_.enqueueNotification(new SaveStateNotification());
}
void XPLHal::SaveStateDeterminators()
{
    save_state_request_queue_.enqueueNotification(new SaveStateNotification(true,false));
}
void XPLHal::SaveStateGlobals()
{
    save_state_request_queue_.enqueueNotification(new SaveStateNotification(false,true));
}

void XPLHal::saveDeterminators ( void )
{
    poco_debug ( hallog, "trying to save determinators" );
    ruleMgr->saveDeterminators();
}


void XPLHal::startXHCP()
{


    ServerSocket sckt ( 3865 );
    try {
    //     srv.assign(new TCPServer(new TCPServerConnectionFactoryImpl<XHCPServerConnection>(ruleMgr), sckt));
    //     cout << "dispactchmaster: " << dispatch << " pt " << dispatch.get() << "\n";
        poco_debug ( hallog, "XHCP connection listerner set up" );
        srv.assign ( new TCPServer ( new XHCPServerConnectionFactory ( this ), sckt ) );
        //TCPServer srv(new XHCPServerConnectionFactory(), sckt);

        srv->start();
        poco_debug ( hallog, "XHCP server started at " + sckt.address().toString() );
    //     cout << "XHCP server started (?) at " << sckt.address().toString() <<  "\n";
    } catch (NetException & e ) {
        poco_debug ( hallog, "Failed to start XHCP" );
    }

}


Path XPLHal::getConfigFileLocation()
{
    Poco::Path p ( Poco::Path::home() );
    p.pushDirectory ( ".xPL" );
    p.pushDirectory ( "xplhallite" );
    return p;
}


