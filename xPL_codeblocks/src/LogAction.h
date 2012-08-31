#ifndef LogAction_H
#define LogAction_H

#include <vector>
#include "XPLMessage.h"
#include "DeterminatorAction.h"
#include "pugixml/pugixml.hpp"

using namespace std;
static const string tabLevel = "\t\t";

class LogAction : public DeterminatorAction {
  public:
    LogAction(string name, string message);
    LogAction(pugi::xml_node);
    ~LogAction();
    void execute();
    void appendAction(pugi::xml_node* outputnode);
    bool equals(LogAction* action);
    string messageText;
    
  protected:

  private:

    
};

#endif //LogAction_H
