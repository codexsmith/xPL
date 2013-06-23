
#include "XHCPServerConnection.h"
#include "XHCPDispatcher.h"
#include "XPLHal.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Thread.h"
#include "Poco/String.h"

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



XHCPServerConnection::XHCPServerConnection ( const StreamSocket& s , XPLHal* halin ) : TCPServerConnection ( s )
{
    theMap["LISTRULES"] = &XHCPDispatcher::listRules;
    theMap["LISTRULEGROUPS"] = &XHCPDispatcher::listRuleGroups;

    theMap["DELGLOBAL"]= &XHCPDispatcher::delGlobal;
    theMap["GETGLOBAL"]= &XHCPDispatcher::getGlobal;
    theMap["LISTGLOBALS"]= &XHCPDispatcher::listGlobals;
    theMap["SETGLOBAL"]= &XHCPDispatcher::setGlobal;

    theMap["DELRULE"]= &XHCPDispatcher::delRule;
    theMap["GETRULE"]= &XHCPDispatcher::getRule;
    theMap["SETRULE"]= &XHCPDispatcher::setRule;
    theMap["RUNRULE"]= &XHCPDispatcher::runRule;

    theMap["CAPABILITIES"]= &XHCPDispatcher::capCommand;
    theMap["LISTOPTIONS"]= &XHCPDispatcher::listOptions;
    theMap["LISTDEVICES"]= &XHCPDispatcher::listDevices;
    theMap["LISTSUBS"]= &XHCPDispatcher::listSubs;
    theMap["QUIT"]= &XHCPDispatcher::quit;
    
    theMap["ADDEVENT"]= &XHCPDispatcher::addEvent;

    hal = ( halin );
    cout << "dispactchconinit: " << hal << "\n";

}

void XHCPServerConnection::run()
{
    StreamSocket& ss = socket();
    SocketStream strm ( ss );
    try
    {
        strm << "200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n";
        cout << "server: 200 GA-TECH-XPLHAL.SERVER1 Version 0.0 alpha XHCP 1.5 ready\r\n";
        strm.flush();
        while ( strm.good() )
        {
            string line;
            string command;
            string args = "";
            getline ( strm,line );
            cout << "client: " << line << "\n";
            trimInPlace ( line );


            if ( line.length() < 2 )
            {
//                 if(line.length()==0) {
//                     return;
//                 }
                continue;
            }

            int split = line.find_first_of ( " " );

            command = line.substr ( 0,split );
            trimInPlace ( command );
            toUpperInPlace ( command );
            if ( split < line.length() )
            {
                if ( split + 1 < line.length() )
                {
                    args = line.substr ( split+1 );
                    trimInPlace ( args );
                }
            }

            string response = "\n";

            //cout << "cmd: \"" << command << "\" : \"" << args <<  "\" \n";

            map<string,pt2Member>::iterator mit = theMap.find ( command );
            if ( mit == theMap.end() )
            {
                cout << "command \"" << command << "\" not in list\n";
                continue;
            }

            response = ( hal->dispatch->*theMap[command] ) ( args, strm );

            strm << response;
            strm.flush();
            cout << "server: " << response;
            if ( response=="221 Closing transmission channel - goodbye.\r\n" )
            {
                return;
            }
        }

    }
    catch ( Poco::Exception& exc )
    {
        std::cerr << "XHCPServerConnection: " << exc.displayText() << std::endl;
    }
}





