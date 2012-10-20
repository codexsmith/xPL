#ifndef DeterminatorAction_H
#define DeterminatorAction_H

#include <vector>
#include <iostream>
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"
using namespace std;

class DeterminatorAction {
  public:

      virtual void execute(DeterminatorEnvironment* env) = 0;
      virtual void appendAction(pugi::xml_node* outputnode) = 0;
    //bool equals(XPLAction* action);
      virtual ~DeterminatorAction(){};
      string display_name;
      int executeOrder;
};

#endif //DeterminatorAction_H
