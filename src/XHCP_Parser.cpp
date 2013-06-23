#include "XHCP_Parser.h"
#include <iostream>
#include <algorithm>

//XHCP_Parser Constructor
//Maps inbound xHCP commands to a function that will create a proper response
XHCP_Parser::XHCP_Parser()
{
    theMap["SETRULE"] = &XHCPDispatcher::addRule;
    theMap["LISTRULES"] = &XHCPDispatcher::listRules;
    theMap["DELRULE"]= &XHCPDispatcher::deleteRule;
    theMap["CAPABILITIES"]= &XHCPDispatcher::capCommand;
    

}

//XHCP_Parser Destructor
XHCP_Parser::~XHCP_Parser()
{
    //dtor
}

//Handler for inbound TCP (3865) messages.  Cleans up string and passes to
//the proper function using theMap
void XHCP_Parser::recvMsg ( TCPSocket *pcClientSocket, char *msg, int msgSize )
{
    std::string theString ( msg, msgSize );
    std::string command;
    std::string remainder;
    std::string item;
    std::vector<std::string> theList;


    while ( !theString.empty() && ( ( theString[theString.length()-1] == '\n' ) || ( theString[theString.length()-1] == '\r' ) ) )
    {
        theString.erase ( theString.length()-1 );
    }

    cout << "got line: " << theString << ".\n";
    std::stringstream ss ( theString );
    while ( std::getline ( ss,item, ' ' ) )
        theList.push_back ( item );



    if ( theList.size() < 1 )
    {
        cout << "no line\n";
        return;
    }

    command = theList[0];
    std::transform ( command.begin(), command.end(),command.begin(), ::toupper );

    map<string,pt2Member>::iterator mit = theMap.find ( command );
    if ( mit == theMap.end() )
    {
        cout << "command \"" << command << "\" not in list\n";
        return;
    }

    remainder = theString;
    if ( theString.size() > command.size() + 1 )
    {
        remainder.erase ( 0, command.size() +1 );
    }
    else
    {
        remainder = "";
    }

    std::string buffer = ( aParser->*theMap[command] ) ( remainder );
    int sendMsgSize = buffer.size();
    pcClientSocket->SendData ( buffer.c_str(), sendMsgSize );
}
void XHCP_Parser::acceptMsg ( TCPSocket *pcClientSocket )
{
    std::string greeting ( "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n" );
    int sendMsgSize = greeting.size();
    int iBytesTransferred = 0;
    pcClientSocket->SendData ( greeting.c_str(), sendMsgSize );
}

