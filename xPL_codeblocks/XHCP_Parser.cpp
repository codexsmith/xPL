#include "XHCP_Parser.h"


XHCP_Parser::XHCP_Parser()
{
    theMap["ADDRULE"] = &Dispatcher::addRule;
    theMap["LISTRULE"] = &Dispatcher::listRule;
    theMap["DELETERULE"]= &Dispatcher::deleteRule;
}
}

XHCP_Parser::~XHCP_Parser()
{
    //dtor
}
void XHCP_Parser::recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize){
    std::string theString(msg, msgSize);
    std::string command;
    while(*msg != '\n'){
        command += *msg;
        msg++;
    }
    Dispatcher aParser;
    std::string buffer = (aParser.*theMap[theString])(theString);
    int sendMsgSize = buffer.size();
    pcClientSocket->SendData(buffer.c_str(), sendMsgSize);

}
void XHCP_Parser::acceptMSG(TCPSocket *pcClientSocket, char *msg, int msgSize){
    std::string greeting( "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n" );
    int sendMsgSize = greeting.size();
    int iBytesTransferred = 0;
    pcClientSocket->SendData(greeting.c_str(), sendMsgSize);
}
