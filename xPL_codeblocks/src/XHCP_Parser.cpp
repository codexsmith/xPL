#include "XHCP_Parser.h"
#include <iostream>

//XHCP_Parser Constructor
//Maps inbound xHCP commands to a function that will create a proper response
XHCP_Parser::XHCP_Parser(){
    theMap["SETRULE"] = &Dispatcher::addRule;
    theMap["LISTRULE"] = &Dispatcher::listRule;
    theMap["DELRULE"]= &Dispatcher::deleteRule;
    theMap["CAPABILITIES"]= &Dispatcher::capCommand;
}

//XHCP_Parser Destructor
XHCP_Parser::~XHCP_Parser()
{
    //dtor
}

//Handler for inbound TCP (3865) messages.  Cleans up string and passes to
//the proper function using theMap
void XHCP_Parser::recvMsg(TCPSocket *pcClientSocket, char *msg, int msgSize){
    std::string theString(msg, msgSize);
    std::string command;
    std::string item;
    std::vector<std::string> theList;
    std::stringstream ss(theString);
    while(std::getline(ss,item, ' '))
        theList.push_back(item);
    Dispatcher aParser;

    //Check for extra carriage returns and line returns
    if(theList.size() == 1){
        if (theList[0].substr(theList[0].size()-2, 2).compare("\r\n")==0)
            theList[0] = theList[0].substr(0, theList[0].size()-2);
    }
    else if(theList.size() == 2){
        if (theList[1].substr(theList[1].size()-2, 2).compare("\r\n")==0)
            theList[1] = theList[1].substr(0, theList[1].size()-2);
        command = theList[1];
    }

    else command = "No Argument";
    std::string buffer = (aParser.*theMap[theList[0]])(command);
    int sendMsgSize = buffer.size();
    pcClientSocket->SendData(buffer.c_str(), sendMsgSize);
}
void XHCP_Parser::acceptMsg(TCPSocket *pcClientSocket){
    std::string greeting( "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n" );
    int sendMsgSize = greeting.size();
    int iBytesTransferred = 0;
    pcClientSocket->SendData(greeting.c_str(), sendMsgSize);
}

