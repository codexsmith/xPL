// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>
#include "XPLCommon.h"
#include "XPLRuleManager.h"

#include "Poco/SharedPtr.h"
#include "xplUDP.h"
#include "xplComms.h"
#include "xplDevice.h"

using namespace Poco;
using namespace xpl;
/*
extern "C" {

    #include "xPLLib/xPL.h"

}*/

using namespace std;





class XPLHal {
    public:
        XPLHal();
        ~XPLHal();
        void saveDeterminators();
        XPLRuleManager ruleMgr;
        
        void HandleDeviceMessages(MessageRxNotification*);
        void HandleAllMessages(MessageRxNotification*);
        
    private:
        xplUDP* myComms;
        //xplDevice* pDevice;
        SharedPtr<xplDevice> pDevice;

};


#endif // XPLHal_H
