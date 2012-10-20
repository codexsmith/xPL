#ifndef XPLRuleManager_H
#define XPLRuleManager_H

//#include "XPLMessage.h"
#include <vector>
#include <map>
#include "Determinator.h"
#include "XPLCommon.h"
#include "xplMsg.h"
#include "Poco/RWLock.h"
#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Runnable.h"
#include "Poco/NotificationQueue.h"
#include <Poco/Thread.h>
#include "Poco/Timer.h"
#include "Poco/Timestamp.h"
//class XPLMessage;

class Determinator;

using namespace std;
using namespace xpl;
using namespace Poco;




// The notification sent to us about an event
class DetetminatorEventNotification: public Notification
{
public:
    typedef AutoPtr<DetetminatorEventNotification> Ptr;
    DeterminatorEnvironment env;
    DetetminatorEventNotification ( DeterminatorEnvironment envin ) : env ( envin ) {  }
};

// quit notification sent to make our worked thread quit
class QuitNotification: public Notification
{
public:
    typedef AutoPtr<QuitNotification> Ptr;
};



class XPLRuleManager : public Runnable
{
public:

    static XPLRuleManager& instance();


    XPLRuleManager ( std::map< string, Determinator* >* determinators );
    XPLRuleManager( );

    ~XPLRuleManager();


    void match ( DeterminatorEnvironment& env );
    std::string detToString();//XHCP support
    Determinator* retrieveDeterminator ( string GUID );
    //takes ownership
    void setDeterminator ( string GUID, Determinator* detin );
    bool removeDeterminator ( string GUID );
    bool runDeterminator ( string GUID );

    void saveDeterminators();

    void run();

    NotificationQueue determinatorEventQueue;

    //TODO make these private and give other classes some magic way to touch them
    RWLock detLock;
    map< string, Determinator*>* determinators;

private:
    Logger& rulelog;
    static const string saveLocation ;
    void loadDeterminators( );
    static XPLRuleManager* m_pInstance;


    Thread eventThread;
    Timer determinatorEventTimer;
    Timestamp lastDeterminatorTimeEvent;
    void timerCallback ( Poco::Timer& timer );
};

#endif //XPLRuleManager_H
