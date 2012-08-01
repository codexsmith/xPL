#ifndef XHCP_PARSER_H
#define XHCP_PARSER_H
#include <map>
#include "TCPServer.h"
#include "Dispatcher.h"
#include "XPLRuleManager.h"

typedef std::string (Dispatcher::*pt2Member)(std::string);
class XHCP_Parser
{
    public:
        static XHCP_Parser* Create()
        {
            if(!singleton){
                singleton = new XHCP_Parser;
            }
            return singleton;
        }
        virtual ~XHCP_Parser();
        void recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize);
        void acceptMsg(TCPSocket *pcClientSocket);

    protected:
    private:
        std::map<std::string,pt2Member> theMap;
        static XHCP_Parser* singleton;
        XHCP_Parser();

};


#endif // XHCP_PARSER_H
