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
#include "XPLMessage.h"
#include "XPLParser.h"
#include "XPLRuleManager.h"
#include "Determinator.h"
#include "DeterminatorAction.h"
#include "XPLAction.h"
#include "XPLCondition.h"

#include "Poco/Thread.h"

using namespace Poco;

/******************************************************************************/
/* Include header files for TCPServer and StreamerThread classes.             */
/******************************************************************************/
#include "TCPDaemon.h"

extern "C" {

    #include "xPLLib/xPL.h"

}

//Prototypes
void* xHCPService(void*);
vector<Determinator>* createDeterminator();

pthread_t xHCP_thread;
xPL_ServicePtr theService = NULL;
XPLRuleManager* ruleMgr;
// disxhcp  Daemon cDaemon(CONFIG_FILE);




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
    closelog();
    exit(0); 
    
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

    while(1) {
        
        Thread::sleep(1000);
        cout << "test app sleeping\n";
        
    }

	closelog();
	
	
  //cout << "del rule manager: " << ruleMgr << " \n";
  //ruleMgr->saveDeterminators();
   // return 0;
    exit(0);
}

Condition xplCond;

void* xplService(void*)
{
    const std::string srcVendor = "HAL9000";
    const std::string srcDeviceID = "xPLHAL";
    const std::string srcInstanceID = "1";
    
    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL\n");
        //exit(1);
    }
    
    /* And a listener for all xPL messages */
    xPL_addMessageListener(XPLParser::recvMsg, NULL);
    
    /* Create a service so the hubs know to send things to us        */
    /* While we are not really using the service, xPL hubs will not  */
    /* forward messages to us until they have seen an xPL-looking    */
    /* device on the end of a hub connection. So this just gets us a */
    /* place at the table, so to speak                               */
    theService = xPL_createConfigurableService(srcVendor.c_str(), srcDeviceID.c_str(), "hal.xpl");
    //theService = xPL_createConfigurableService(srcVendor.c_str(),"b", "hal.xpl");
    xPL_setServiceVersion(theService, HAL_VERSION);
    
    xPL_setServiceEnabled(theService, TRUE);
    
    /* Hand control over to xPLLib */
    /* Main thread is processing xPL messages */
    /* xHCP_thread is processing xHCP messages */
    
    xplCond.LockMutEx();
//     while (!bKillFlag)
    {
        xPL_processMessages(100);
        xplCond.Wait();
//         xplCond.
//         std::cout << "xsignaled.\n";

    }
    std::cout << "xkilling.\n";
    xplCond.UnlockMutEx();
    std::cout << "xkilling2.\n";
    
    //syslog(LOG_INFO, "Exiting xHCP Thread");
//     Stop();
    std::cout << "xkilling3.\n";
    pthread_exit(NULL);
    
    

    cout << "xpl processing down\n";
    
}


//The xHCP thread executes this function
void* xHCPService(void*)
{

    // disxhcp  cDaemon.RunDaemon();
}


