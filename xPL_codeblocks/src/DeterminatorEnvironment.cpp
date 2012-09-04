

#include "XPLCondition.h"
#include "XPLHal.h"
#include "XPLMessage.h"


#include <string>
#include <iostream>

using namespace std;

public:
DeterminatorEnvrionment::DeterminatorEnvrionment() {
  message = null;
}
DeterminatorEnvrionment::DeterminatorEnvrionment(XPLMessage* messagein) {
  message = messagein;
}

