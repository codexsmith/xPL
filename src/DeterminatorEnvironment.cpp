
#include "DeterminatorEnvironment.h"
#include "XPLCondition.h"
#include "XPLCommon.h"
#include "XPLHal.h"


#include <string>
#include <iostream>

using namespace std;


DeterminatorEnvironment::DeterminatorEnvironment() {
  message = NULL;
  time_t stime = time(0);
  globals = XPLHal::instance().globals;
  envType = none;
}
DeterminatorEnvironment::DeterminatorEnvironment( AutoPtr< xplMsg > messagein) {
  message = messagein;
  time_t stime = time(0);
  globals = (XPLHal::instance()).globals;
  envType = xPLMessage;
}
DeterminatorEnvironment::DeterminatorEnvironment( string globalNameIn) {
    globalName = globalNameIn;
    time_t stime = time(0);
    globals = (XPLHal::instance()).globals;
    envType = globalChanged;
}


