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
    
  protected:
    

  private:
    
};

#endif //DeterminatorAction_H
