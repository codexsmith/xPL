// a manager for the global variables

#ifndef GlobalManager_H
#define GlobalManager_H

#include "GlobalManager.h"

#include "Poco/Mutex.h"
#include <string>
#include <vector>
#include <map>
#include "Poco/Util/XMLConfiguration.h"
#include <Poco/Path.h>
#include <vector>

using Poco::Util::XMLConfiguration;
using Poco::Util::AbstractConfiguration;


using namespace std;
using namespace Poco;

class GlobalManager
{
public:
    GlobalManager();
    ~GlobalManager();
    void saveDeterminators();
    
    static string cleanGlobalName(string name );
    bool hasGlobal(string name);
    string getGlobal(string name);
    bool deleteGlobal(string name);
    
    //gets a copy
    map<string, string> getGlobals();
    void setGlobal(string name, string value);

private:
    void loadGlobals();
    void saveGlobals();
    Path globalvarpath;
    XMLConfiguration *cfg;
    Mutex globalLock;
    map<string, string> globalVars;

};


#endif // GlobalManager_H
