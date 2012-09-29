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
#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"

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
    static XPLHal& createInstance();
    static void deleteInstance() ;
    
    XPLHal();
    ~XPLHal();
    void saveDeterminators();
    SharedPtr<XPLRuleManager> ruleMgr;
    SharedPtr<XHCPDispatcher> dispatch;

    void HandleDeviceMessages ( MessageRxNotification* );
    void HandleAllMessages ( MessageRxNotification* );

    GlobalManager globals;
    static Path getConfigFileLocation();
private:
    xplUDP* myComms;
    
    Logger& hallog;
    
    //xplDevice* pDevice;
    SharedPtr<xplDevice> pDevice;
    SharedPtr<XPLHal> sharedThis;
    SharedPtr<TCPServer> srv;
    void startXHCP();

};


#endif // XPLHal_H
