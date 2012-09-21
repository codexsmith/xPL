#ifndef XHCP_PARSER_H
#define XHCP_PARSER_H
#include <map>
#include <sstream>
#include "TCPServer.h"
#include "XHCPDispatcher.h"
#include "XPLRuleManager.h"
#include "Poco/SharedPtr.h"

typedef std::string (XHCPDispatcher::*pt2Member)(std::string);
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
        SharedPtr<XHCPDispatcher> aParser;

};


#endif // XHCP_PARSER_H
