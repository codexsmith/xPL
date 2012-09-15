#ifndef XPLParser_H
#define XPLParser_H

#include "XPLMessage.h"

#include "Poco/SharedPtr.h"

class XPLRulesManager;

extern "C" {

    #include "xPLLib/xPL.h"

}

using namespace Poco;
using namespace std;

class XPLParser
{
    //PUBLIC//
    public:
    
    
	//FUNCTION PROTOTYPES
    //Constructor
    XPLParser();
    
    //Destructor
    ~XPLParser();
    
    static const XPLParser& instance();

    static void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue);

    static int sendMsg(XPLMessage msg);
    
    private:

        XPLParser(XPLParser const&) {};
        XPLParser& operator=(XPLParser const&) {};
        static XPLParser* m_pInstance;
    SharedPtr< XPLRulesManager > rulesMgr;
        
};

#endif // XPLParser_h

