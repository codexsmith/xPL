// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>
#include "XPLCommon.h"
#include "XPLRuleManager.h"
#include "GlobalManager.h"

#include "Poco/SharedPtr.h"
#include "XplUDP.h"
#include "XplComms.h"
#include "XplDevice.h"
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


// The notification sent to us to save our state
class SaveStateNotification: public Notification 
{
public:
    typedef AutoPtr<SaveStateNotification> Ptr;
    SaveStateNotification(bool save_determinators, bool save_globals):save_determinators_(save_determinators),save_globals_(save_globals){};
    SaveStateNotification():save_determinators_(true),save_globals_(true){};
    bool save_determinators_;
    bool save_globals_;
};


class XPLHal
{
public:
    static XPLHal& instance();
    static XPLHal& createInstance();
    static void deleteInstance() ;

    XPLHal();
    ~XPLHal();

    void start();
    void saveDeterminators();
    SharedPtr<XPLRuleManager> ruleMgr;
    SharedPtr<XHCPDispatcher> dispatch;

    void HandleDeviceMessages ( MessageRxNotification* );
    void HandleAllMessages ( MessageRxNotification* );
    /**
     * @brief Called when the XPL Hal needs to save out it's state to disk. Can be called from any thread.
     **/
    void SaveState ();
    void SaveStateGlobals ();
    void SaveStateDeterminators ();
    void SaveStateLoop();
    
    NotificationQueue save_state_request_queue_;
    
    SharedPtr<GlobalManager> globals;
    static Path getConfigFileLocation();
private:
    XplUDP* myComms;

    Logger& hallog;

    //XplDevice* pDevice;
    SharedPtr<XplDevice> pDevice;
    SharedPtr<XPLHal> sharedThis;
    SharedPtr<TCPServer> srv;
    RunnableAdapter< XPLHal >* save_adapter_;
    Thread save_thread_;
    void startXHCP();

};


#endif // XPLHal_H
