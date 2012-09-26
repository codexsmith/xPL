// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>
#include "XPLCommon.h"
#include "XPLRuleManager.h"
#include "GlobalManager.h"

#include "Poco/SharedPtr.h"
#include "xplUDP.h"
#include "xplComms.h"
#include "xplDevice.h"
#include "XHCPDispatcher.h"
#include "Poco/Net/TCPServer.h"

using namespace Poco;
using namespace xpl;
using Poco::Net::TCPServer;
/*
extern "C" {

    #include "xPLLib/xPL.h"

}*/

using namespace std;





class XPLHal
{
public:
    static XPLHal& instance();
    
    XPLHal();
    ~XPLHal();
    void saveDeterminators();
    SharedPtr<XPLRuleManager> ruleMgr;
    SharedPtr<XHCPDispatcher> dispatch;

    void HandleDeviceMessages ( MessageRxNotification* );
    void HandleAllMessages ( MessageRxNotification* );

    GlobalManager globals;
    
private:
    xplUDP* myComms;
    
    //xplDevice* pDevice;
    SharedPtr<xplDevice> pDevice;
    SharedPtr<XPLHal> sharedThis;
    SharedPtr<TCPServer> srv;
    void startXHCP();

};


#endif // XPLHal_H
