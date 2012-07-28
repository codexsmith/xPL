#ifndef XHCP_PARSER_H
#define XHCP_PARSER_H
#include <map>
#include "tcpserver.h"
#include "Dispatcher.h"
#include "XPLRuleManager.h"

typedef std::string (Dispatcher::*pt2Member)(std::string);
class XHCP_Parser
{
    public:
        XHCP_Parser();
        virtual ~XHCP_Parser();
        static void recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize);
        static void acceptMsg(TCPSocket *pcClientSocket);
        static std::map<std::string,pt2Member> theMap;
    protected:
    private:
        void run();
};


#endif // XHCP_PARSER_H
