#ifndef XPLParser_H
#define XPLParser_H

#include "XPLMessage.h"

extern "C" {

    #include "xPLLib/xPL.h"

}

using namespace std;

class XPLParser
{
    //PUBLIC//
    public:
    static XPLParser* Instance();
    
	//FUNCTION PROTOTYPES



    static void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue);

    static int sendMsg(XPLMessage msg);
    
    private:
        //Constructor
        XPLParser();
        
        //Destructor
        ~XPLParser();
        
        XPLParser(XPLParser const&) {};
        XPLParser& operator=(XPLParser const&) {};
        static XPLParser* m_pInstance;
        
};

#endif // XPLParser_h

