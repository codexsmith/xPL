#ifndef XPLRuleManager_H
#define XPLRuleManager_H

//#include "XPLMessage.h"
#include <vector>
#include <map>
#include "Determinator.h"
#include "XPLCommon.h"
#include "xplMsg.h"
#include "Poco/RWLock.h"

//class XPLMessage;

class Determinator;

using namespace std;
using namespace xpl;
using namespace Poco;

class XPLRuleManager {
	public:

    static XPLRuleManager& instance();
      
    void match(xplMsg&);
        std::string detToString();//XHCP support
        Determinator* retrieveDeterminator(string GUID);
        //takes ownership
        void setDeterminator(string GUID, Determinator* detin);
        bool removeDeterminator(string GUID);
        bool runDeterminator(string GUID);
        
	void saveDeterminators();

  XPLRuleManager( std::map< string, Determinator* >* determinators );
  XPLRuleManager( );

  ~XPLRuleManager();
  
  //TODO make these private and give other classes some magic way to touch them
  RWLock detLock;
  map< string, Determinator*>* determinators;
  
	private:
    
    static const string saveLocation ;
    void loadDeterminators( );
    static XPLRuleManager* m_pInstance;
};

#endif //XPLRuleManager_H
