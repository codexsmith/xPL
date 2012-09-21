
#include "XHCPServerConnection.h"
#include "XHCPDispatcher.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Thread.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnectionFactoryImpl;
using Poco::Net::TCPServerParams;
using Poco::Net::SocketStream;
using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;



XHCPServerConnection::XHCPServerConnection ( const StreamSocket& s , SharedPtr<XHCPDispatcher> dispatchin ) : TCPServerConnection ( s )
{
    theMap["SETRULE"] = &XHCPDispatcher::addRule;
    theMap["LISTRULES"] = &XHCPDispatcher::listRules;
    theMap["DELRULE"]= &XHCPDispatcher::deleteRule;
    theMap["CAPABILITIES"]= &XHCPDispatcher::capCommand;
    dispatch = dispatchin;
}

void XHCPServerConnection::run()
{
    StreamSocket& ss = socket();
    SocketStream strm (ss);
    try
    {
        while (1) {
            string line;
            getline(strm,line);
            cout << "got line: " << line << ".\n";
            
            istringstream iss(line);
            vector<string> tokens;
            copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter<vector<string> >(tokens));
            
            cout << "tokens: " << tokens.size() << "\n";
            
            string response = "\n";
            
            if(tokens.size()) {
                cout << "cmd: " << tokens[0] << "\n";
               // response = dispatch->*theMap[command])(tokens);
            }
            
            
            
//             std::transform(command.begin(), command.end(),command.begin(), ::toupper);
//             
//             map<string,pt2Member>::iterator mit = theMap.find(command);
//             if (mit == theMap.end()){
//                 cout << "command \"" << command << "\" not in list\n";
//                 return;
//             }
//             
//             remainder = theString;
//             if (theString.size() > command.size() + 1) {
//                 remainder.erase(0, command.size()+1);
//             } else {
//                 remainder = "";
//             }
//             
//             string response = (dispatch->*theMap[command])(remainder);
            
            strm << response;
        }
        
        
      /*  
        std::string greeting( "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n" );
        int sendMsgSize = greeting.size();
        ss.sendBytes(greeting.c_str(), sendMsgSize);
        
        char buffer[1024];
        
        while (ss.)
        {
            
            std::string theString(buffer, n);
            std::string command;
            std::string remainder;
            std::string item;
            std::vector<std::string> theList;
            
            
            while (!theString.empty() && ((theString[theString.length()-1] == '\n') || (theString[theString.length()-1] == '\r'))) {
                theString.erase(theString.length()-1);
            }
            
            cout << "got line: " << theString << ".\n";

            ss.re
            
            while(std::getline(ss,item, ' '))
                theList.push_back(item);
            
            
            
            if(theList.size() < 1){
                cout << "no line\n";
                return;
            }
            
            command = theList[0];
            std::transform(command.begin(), command.end(),command.begin(), ::toupper);
            
            map<string,pt2Member>::iterator mit = theMap.find(command);
            if (istringstream iss(sentence);mit == theMap.end()){
                cout << "command \"" << command << "\" not in list\n";
                return;
            }
            
            remainder = theString;
            if (theString.size() > command.size() + 1) {
                remainder.erase(0, command.size()+1);
            } else {
                remainder = "";
            }
            
            std::string buffer = (dispatch->*theMap[command])(remainder);
            int sendMsgSize = buffer.size();
            ss.writeBytes(buffer.c_str(),sendMsgSize);
       
            n = ss.read ( buffer, sizeof ( buffer ) );
        }*/
    }
    catch ( Poco::Exception& exc )
    {
        std::cerr << "XHCPServerConnection: " << exc.displayText() << std::endl;
    }
}





