#ifndef XHCP_PARSER_H
#define XHCP_PARSER_H
#include "tcpserver.h"

class XHCP_Parser
{
    public:
        XHCP_Parser();
        virtual ~XHCP_Parser();
        static void recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize);
        static void acceptMsg(TCPSocket *pcClientSocket);
    protected:
    private:
};

#endif // XHCP_PARSER_H
