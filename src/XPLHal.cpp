#include "XPLHal.h"


#include <vector>
#include <string>
#include <iostream>

#include "XPLAction.h"
#include "XPLParser.h"
#include <xplMsg.h>



using namespace std;

XPLHal::XPLHal() {
    cout << "XPLHal Created\n";
    //Load XML Determinators from disk.
    //ruleMgr = new XPLRuleManager();
    
    
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
    
    
    
}
XPLHal::~XPLHal() {
     cout << "XPLHal Destroying\n";
    cout << "XPLHal Destroyed\n";
}

//messages that are local to us
void XPLHal::HandleDeviceMessages(MessageRxNotification* mNot) {
    cout << "got directed message: " << mNot->message->GetSchemaClass() << " " <<mNot->message->GetSchemaType() << "\n";
    mNot->release();
}

//messages for anyone, to be processed by the rules engine
void XPLHal::HandleAllMessages(MessageRxNotification* mNot) {
    cout << "got message: " << mNot->message->GetSchemaClass() << " " <<mNot->message->GetSchemaType() << "\n";
    ruleMgr.match(*(mNot->message));
    
    cout<<"here1 " << mNot->message->GetSource().device << "\n";
    mNot->release();
    cout<<"here2\n";
}


void XPLHal::saveDeterminators(void) {
    cout << "trying to save determinators (hal)\n";
    ruleMgr.saveDeterminators();
}