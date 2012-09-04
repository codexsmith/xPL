#include "XHCP_Parser.h"
#include <iostream>
#include <algorithm>

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
    
    
    while (!theString.empty() && ((theString[theString.length()-1] == '\n') || (theString[theString.length()-1] == '\r'))) {
        theString.erase(theString.length()-1);
    }
    
    cout << "got line: " << theString << ".\n";
    std::stringstream ss(theString);
    while(std::getline(ss,item, ' '))
        theList.push_back(item);
    Dispatcher aParser;

    
    if(theList.size() < 1){
        cout << "no line\n";
        return;
    }
    map<string,pt2Member>::iterator mit = theMap.find(theList[0]);
    if (mit == theMap.end()){
        cout << "command \"" << theList[0] << "\" not in list\n";
        return;
    }
    
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

