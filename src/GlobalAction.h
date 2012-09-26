#ifndef GlobalAction_H
#define GlobalAction_H

#include <vector>
#include "DeterminatorAction.h"
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"

using namespace std;

class GlobalAction : public DeterminatorAction {
  public:
    GlobalAction(string name, string globalName, string globalValue);
    GlobalAction(pugi::xml_node);
    ~GlobalAction();
    void execute(DeterminatorEnvironment* env);
    void appendAction(pugi::xml_node* outputnode);
    bool equals(GlobalAction* action);
    
    
  protected:

  private:
      string globalName;
      string globalValue;
    
};

#endif //GlobalAction_H
