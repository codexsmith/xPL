// xPL Linux Hal Server basic types

#ifndef XPLHal_H
#define XPLHal_H

#include <string>
#include <vector>
#include "XPLCommon.h"
#include "XPLRuleManager.h"

#include "Poco/SharedPtr.h"
//#include "xplUDP.h"
#include "xplComms.h"
using namespace Poco;

extern "C" {

    #include "xPLLib/xPL.h"

}

using namespace std;





class XPLHal {
    public:
        XPLHal();
        ~XPLHal();
        void saveDeterminators();
        Poco::SharedPtr<XPLRuleManager> ruleMgr;
    private:
        xplComms;
};


#endif // XPLHal_H
