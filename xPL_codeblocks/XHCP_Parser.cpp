#include "XHCP_Parser.h"


XHCP_Parser::XHCP_Parser(){
    theMap["ADDRULE"] = &Dispatcher::addRule;
    theMap["LISTRULE"] = &Dispatcher::listRule;
    theMap["DELETERULE"]= &Dispatcher::deleteRule;
    theMap["CAPABILITIES"]= &Dispatcher::capCommand;
}

XHCP_Parser::~XHCP_Parser()
{
    //dtor
}
void XHCP_Parser::recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize){
    std::string theString(msg, msgSize);
    std::string command;
    std::string item;
    std::vector<std::string> theList;
    std::stringstream ss(theString);
    while(std::getline(ss,item, ' '))
        theList.push_back(item);
    Dispatcher aParser;
    if(theList.size == 2){
        command = list[1];
    }
    else command = "No Argument";
    std::string buffer = (aParser.*theMap[theList[0]])(command);
    pcClientSocket->SendData(greeting.c_str(), sendMsgSize);
}
void XHCP_Parser::acceptMsg(TCPSocket *pcClientSocket){
    std::string greeting( "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n" );
    int sendMsgSize = greeting.size();
    int iBytesTransferred = 0;
    pcClientSocket->SendData(greeting.c_str(), sendMsgSize);
}

