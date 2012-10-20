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
#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"


using Poco::Util::XMLConfiguration;
using Poco::Util::AbstractConfiguration;


using namespace std;
using namespace Poco;

class GlobalManager
{
public:
    GlobalManager();
    ~GlobalManager();

    GlobalManager& operator= ( const GlobalManager& );
    GlobalManager ( const GlobalManager& );

    void saveDeterminators();
    static string cleanGlobalName ( string name );
    bool hasGlobal ( string name );
    string getGlobal ( string name );
    bool deleteGlobal ( string name );

    //gets a copy
    map<string, string> getGlobals() const;
    void setGlobal ( string name, string value );
    void loadGlobals();
    void saveGlobals();
private:
    Logger& globallog;
    Path globalvarpath;
    mutable Mutex globalLock; //this is mutable to allow getGlobals to be const
    map<string, string> globalVars;

};


#endif // GlobalManager_H
