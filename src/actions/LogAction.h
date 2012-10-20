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

class LogAction : public DeterminatorAction
{
public:
    /**
     * @param namein The friendly name for the XML
     * @param messagein The message to actually be logged when run
     **/
    LogAction ( string name, string message );
    /**
     * @brief Used to generate the LogAction from the a determinator's XML
     * @param actionnode the action node from a determinator's XML
     **/
    LogAction ( pugi::xml_node );
    ~LogAction();
    void execute ( DeterminatorEnvironment* env );
    void appendAction ( pugi::xml_node* outputnode );
    bool equals ( LogAction* action );
    string messageText;

protected:

private:

    Logger& actlog;


};

#endif //LogAction_H
