#ifndef XPLRuleManager_H
#define XPLRuleManager_H

//#include "XPLMessage.h"
#include <vector>
#include "Determinator.h"
#include "XPLCommon.h"
#include "xplMsg.h"
#include "Poco/Mutex.h"

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
	void saveDeterminators();
  Mutex determinatorsMutex;
  vector<Determinator*>* determinators;
  XPLRuleManager();
  XPLRuleManager(std::vector< Determinator* >* );

  ~XPLRuleManager();
  
	private:
      
    
      static const string saveLocation ;
        void loadDeterminators( vector< Determinator*>*);
        static XPLRuleManager* m_pInstance;
};

#endif //XPLRuleManager_H
