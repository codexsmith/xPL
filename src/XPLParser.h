#ifndef XPLParser_H
#define XPLParser_H



#include "Poco/SharedPtr.h"

class XPLRulesManager;


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

    //static void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue); FIXME

    //static int sendMsg(XPLMessage msg); FIXME

private:

    XPLParser ( XPLParser const& ) {};
    XPLParser& operator= ( XPLParser const& ) {};
    static XPLParser* m_pInstance;
    SharedPtr< XPLRulesManager > rulesMgr;

};

#endif // XPLParser_h

