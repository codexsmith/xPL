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

	//FUNCTION PROTOTYPES

    //Constructor
    XPLParser();

    //Destructor
    ~XPLParser();

    static void recvMsg(xPL_MessagePtr theMessage, xPL_ObjectPtr userValue);

    static int sendMsg(XPLMessage msg);

};

#endif // XPLParser_h
