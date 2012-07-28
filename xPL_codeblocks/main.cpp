// xPL Linux Hal Server
#define HAL_VERSION "1.0"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "XPLHal.h";
#include "XPLMessage.h"
#include "XPLParser.h"
#include "XPLRuleManager.h"
#include "Determinator.h"
#include "DeterminatorFactory.h"

extern "C" {

    #include "xPLLib/xPL.h"

}
//#include <boost/asio.hpp>

xPL_ServicePtr theService = NULL;
XPLRuleManager* ruleMgr;

int main(int argc, string argv[])
{
    string input;
    DeterminatorFactory factory;
    bool more = true;
    vector<Determinator>* determinators = new vector<Determinator>;
    Determinator* determinator = factory.createDeterminator(argv);
    if(*determinator != NULL)
        determinators->push_back(*determinator);

    cout << "Define more determinators?\n";
    getline(cin, input);
    if((input.compare("N") == 0) || (input.compare("n")))
        more = false;

    while(more)
    {
       cout << "Define determinator: ";

    }



    /**** Test Code ****/
    XPLMessage testMsg, testMsg2;
    testMsg.addMember("Fruit", "Dealer");
    testMsg.addMember("SK", "MC");
    testMsg.setHops(5);
    testMsg.setSource("Pin", "Tin", "Sin");
    testMsg.setMsgType("Command");

    testMsg2 = testMsg.copyMessage();
    testMsg2.addMember("StarTale", "July");
    testMsg2.setMsgType("Status");

    testMsg.setSource("Poop", "Scoop", "Loop");
    testMsg.setHops(8);

    //Load XML document

    //Initialize RuleManager object with returned vector of determinators

    //Source Address
    String srcVendor = "Test";
    String srcDeviceID = "hal";
    String srcInstanceID = "322F";

    /* Start xPL up */
    if (!xPL_initialize(xPL_getParsedConnectionType())) {
        fprintf(stderr, "Unable to start xPL");
        exit(1);
    }

    /* And a listener for all xPL messages */
    xPL_addMessageListener(XPLParser::recvMsg, NULL);

    /* Create a service so the hubs know to send things to us        */
    /* While we are not relaly using he service, xPL hubs will not   */
    /* forward messages to us until they have seen a xPL-looking     */
    /* device on the end of a hub connection, so this just gets us a */
    /* place at the table, so to speak                               */
    theService = xPL_createConfigurableService(srcVendor, srcDeviceID, "Test.hal");
    xPL_setServiceVersion(theService, HAL_VERSION);

    xPL_setServiceEnabled(theService, TRUE);

    /* Hand control over to xPLLib */
    xPL_processMessages(-1);

    return TRUE;
}
