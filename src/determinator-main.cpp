// xPL Linux Hal Server
#define HAL_VERSION "1.0"

#define CONFIG_FILE "xHCP.config"

#include "projconfig.h"

#include <string>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <iostream>
#include <fstream>
#include "XPLHal.h"
#include "XPLParser.h"
#include "XPLRuleManager.h"
#include "Determinator.h"
#include "DeterminatorAction.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "Poco/Logger.h"

#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"

#include "Poco/Thread.h"

using namespace Poco;

Logger& rootlogger = Logger::root();

//Prototypes
void* xHCPService(void*);
vector<Determinator>* createDeterminator();

// pthread_t xHCP_thread;
// xPL_ServicePtr theService = NULL;
// XPLRuleManager* ruleMgr;
// disxhcp  Daemon cDaemon(CONFIG_FILE);

bool running = true;



void shutdown_handler(int s){
    
    poco_information(rootlogger, "caught signal" + s);

    running = false;
}

void setup_singnal_handler() {
    struct sigaction sigIntHandler;
    
    sigIntHandler.sa_handler = shutdown_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    
    sigaction(SIGINT, &sigIntHandler, NULL);
}

int main(int argc,const char * argv[])
{
    
    
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    //pPF->setProperty("pattern", "%p %I-%T %H:%M:%S %s: %t");
    pPF->setProperty("pattern", "%H:%M:%S %s %I-%T %p: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pCons));
    Logger::root().setChannel(pFC);
    
    rootlogger.setLevel("debug");
    
    poco_warning(rootlogger, "starting logger");
    
    
    poco_information(rootlogger, "Starting up version 1.1");
    
    openlog("xplhallite", LOG_PID, LOG_DAEMON);
    
    //XPLHal& hal = XPLHal::instance();
    XPLHal& hal = XPLHal::createInstance();
    
    
    //pthread_create(&xHCP_thread,NULL,&xHCPService, NULL);
    
    poco_information(rootlogger, "Main thread created");
    
    setup_singnal_handler();
    //cout << "rule manager addr: " << ruleMgr << " \n";
    //Source Address

    while(running) {
        
        Thread::sleep(1000);
        //cout << "test app sleeping\n";
        
    }
  
  XPLHal::deleteInstance();
  closelog();
    return 0;
   // exit(0);
}





//The xHCP thread executes this function
void* xHCPService(void*)
{

    // disxhcp  cDaemon.RunDaemon();
}


