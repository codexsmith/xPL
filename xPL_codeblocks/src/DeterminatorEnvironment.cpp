
#include "DeterminatorEnvironment.h"
#include "XPLCondition.h"
#include "XPLCommon.h"
#include "XPLMessage.h"


#include <string>
#include <iostream>

using namespace std;


DeterminatorEnvironment::DeterminatorEnvironment() {
  message = NULL;
  time_t stime = time(0);
  mtime = localtime(&stime);
}
DeterminatorEnvironment::DeterminatorEnvironment( xplMsg* messagein) {
  message = messagein;
  time_t stime = time(0);
  mtime = localtime(&stime);
}

