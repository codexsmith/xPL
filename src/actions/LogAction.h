#ifndef LogAction_H
#define LogAction_H

#include <vector>
#include "DeterminatorAction.h"
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"
#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"

using namespace std;
static const string tabLevel = "\t\t";

class LogAction : public DeterminatorAction {
  public:
    LogAction(string name, string message);
    LogAction(pugi::xml_node);
    ~LogAction();
    void execute(DeterminatorEnvironment* env);
    void appendAction(pugi::xml_node* outputnode);
    bool equals(LogAction* action);
    string messageText;
    
  protected:

  private:
      
      Logger& actlog;
      
    
};

#endif //LogAction_H
