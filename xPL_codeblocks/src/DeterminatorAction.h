#ifndef DeterminatorAction_H
#define DeterminatorAction_H

#include <vector>
#include <iostream>
#include "pugixml/pugixml.hpp"
using namespace std;

class DeterminatorAction {
  public:

      virtual void execute() = 0;
      virtual void appendAction(pugi::xml_node* outputnode) = 0;
    //bool equals(XPLAction* action);
      virtual ~DeterminatorAction() {cout << "ttewsres\n";};
      string display_name;
      int executeOrder;
};

#endif //DeterminatorAction_H
