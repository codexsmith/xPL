#ifndef XHCP_PARSER_H
#define XHCP_PARSER_H
#include "tcpserver.h"
#include "Dispatcher.h"
#include "RuleManager.h"

typedef std::string (Dispatcher::*pt2Member)(std::string);
class XHCP_Parser
{
    public:
        XHCP_Parser();
        virtual ~XHCP_Parser();
        static void recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize);
        static void acceptMSG(TCPSocket *pcClientSocket, char *msg, int msgSize);
    protected:
    private:
        static std::map<std::string,pt2Member> theMap;
};


#endif // XHCP_PARSER_H
