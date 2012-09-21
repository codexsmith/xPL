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

#include "Poco/Thread.h"

using namespace Poco;



//Prototypes
void* xHCPService(void*);
vector<Determinator>* createDeterminator();

// pthread_t xHCP_thread;
// xPL_ServicePtr theService = NULL;
// XPLRuleManager* ruleMgr;
// disxhcp  Daemon cDaemon(CONFIG_FILE);

bool running = true;



void shutdown_handler(int s){
    printf("\nCaught signal %d\n",s);
    /*
    xPL_setServiceEnabled(theService, FALSE);
    xPL_releaseService(theService);
    xPL_shutdown();*/
    cout << "xPL service down\n";
    
//     xPL_setServiceEnabled(theService, FALSE);
//     xPL_releaseService(theService);
//     printf("Shutting down xPLLib\n");
//     xPL_shutdown();
//     
    int ret = 0;
    //ret = pthread_kill(xHCP_thread, s);
    cout << "sig hand\n";
    // disxhcp cDaemon.SignalHandler(s);
    cout << "sig hand2\n";

    cout << "join\n";
    //pthread_join(xHCP_thread,NULL);
    // disxhcp ret = pthread_cancel(xHCP_thread);
    cout << "joined\n";

//     saveDeterminators();
//     closelog();
//     exit(0); 
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
    //fprintf(stderr, "Starting up %d.%d \n", xplhallite_VERSION_MAJOR, xplhallite_VERSION_MINOR);
    fprintf(stderr, "Starting up %d.%d \n", 1, 1);
    openlog("xplhallite", LOG_PID, LOG_DAEMON);
    
    XPLHal hal;
    
    
    //pthread_create(&xHCP_thread,NULL,&xHCPService, NULL);
    
    syslog(LOG_INFO, "Main Thread Created.");
    
    setup_singnal_handler();
    //cout << "rule manager addr: " << ruleMgr << " \n";
    //Source Address

    while(running) {
        
        Thread::sleep(1000);
        cout << "test app sleeping\n";
        
    }
  
  closelog();
    return 0;
   // exit(0);
}





//The xHCP thread executes this function
void* xHCPService(void*)
{

    // disxhcp  cDaemon.RunDaemon();
}


