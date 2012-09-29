#include "XPLRuleManager.h"
#include <vector>
#include <fstream>
#include <list>
#include <set>
#include "DeterminatorEnvironment.h"


#include <syslog.h>

#include <sys/types.h>
#include <sys/stat.h>
//#ifdef HAVE_UNISTD_H
#include <unistd.h>
//#endif /* HAVE_UNISTD_H */
//#include <string.h>
//#include "sysstat.h"    /* Fix up for Windows - inc mode_t */
#include <dirent.h>
#include <errno.h>

#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/FileStream.h"
#include "Poco/SingletonHolder.h"
#include "Poco/String.h"

#include "XPLHal.h"

using namespace Poco;

using namespace std;


XPLRuleManager::XPLRuleManager(map<string, Determinator*>* determinators):
rulelog(Logger::get("rulemanager"))
{
    //loadDeterminators();
    this->determinators = determinators;
    //this->determinators = loadDeterminators();
}


XPLRuleManager::XPLRuleManager():
rulelog(Logger::get("rulemanager"))
{
    this->determinators = new map<string, Determinator*>();
    //loadDeterminators(this->determinators );
    loadDeterminators();
    //vector< Determinator >* x = loadDeterminators();
    //this->determinators = x;
    //delete x;
}

XPLRuleManager::~XPLRuleManager()
{
    saveDeterminators();
//     cout << "delete determinators from  "  << this << " ";
    
    detLock.writeLock();
    
    map<string, Determinator*>::iterator i = determinators->begin();
    while( i != determinators->end())
    {
        delete i->second;
        i->second = 0; // I don't think this is strictly necessary...
        ++i;
    }
    determinators->clear();
    delete determinators;
    
    detLock.unlock();
//     cout << "\n";
}


namespace{
    static Poco::SingletonHolder<XPLRuleManager> sh;
}
    
XPLRuleManager& XPLRuleManager::instance() {
    return *sh.get();
}

std::string XPLRuleManager::detToString(){
    std::string theString;
    detLock.readLock();
    for (int i = 0; i < determinators->size();i++){
        //string += determinators -> at(i).getID();
        //string += "\r\n";
    }
    detLock.unlock();
    return theString;
}

Determinator* XPLRuleManager::retrieveDeterminator(string GUID) {
    detLock.readLock();

    if (determinators->count( GUID ) ==1) {
        Determinator* detp = (*determinators)[GUID];
        detLock.unlock();
        return detp;
    }
    detLock.unlock();
    return NULL;
}

void XPLRuleManager::setDeterminator(string GUID, Determinator* detin) {
    detLock.writeLock();
    if (determinators->count( GUID ) ==1) {
        Determinator* detp = (*determinators)[GUID];
        (*determinators)[GUID] = detin;
        delete detp;
    } else {
        (*determinators)[GUID] = detin;
    }
    poco_debug(rulelog, "Modified " + (*determinators)[GUID]->name);
//     cout << "Rulemgr: modified " << (*determinators)[GUID]->name << "\n";
    detLock.unlock();
}

bool XPLRuleManager::removeDeterminator( string GUID ){
    detLock.writeLock();
    map<string,Determinator*>::iterator dit;
    for (dit = determinators->begin(); dit != determinators->end();) {
        if(dit->first == Determinator::cleanGUID(GUID)) {
            Determinator* detp = dit->second;
            determinators->erase(dit);
            delete detp;;
            detLock.unlock();   
            
            poco_debug(rulelog, "Deleted " + GUID);
            return true;
        }
        ++dit;
    }
    
    /*
    if (determinators->count( GUID ) ==1) {
        Determinator* detp = (*determinators)[GUID];
        determinators->erase(detp);
        delete detp;
        detLock.unlock();   
        return true;
    }*/
    detLock.unlock();   
    
    poco_notice(rulelog, "determinator " + GUID + "doesn't exist, so not deleting\n");
    return false;
}
bool XPLRuleManager::runDeterminator( string GUID ){
    detLock.readLock();
    GUID = Determinator::cleanGUID(GUID);
    if (determinators->count( GUID ) ==1) {
        Determinator* detp = (*determinators)[GUID];
        DeterminatorEnvironment env;
        detp->execute(&env); //FIXME this is against spec - we're supposed to check the conditions first?
        detLock.unlock();   
        return true;
    }
    detLock.unlock();   
    poco_notice(rulelog, "determinator " + GUID + "doesn't exist, so not running\n");
    return false;
}





void XPLRuleManager::match(xplMsg& msg)
{
    //cout << "rules engine matching \n\n";
    //match stuff
    
    DeterminatorEnvironment env = DeterminatorEnvironment(&msg);
    detLock.readLock();
    
    for(map<string,Determinator*>::iterator dit = determinators->begin(); dit!=determinators->end(); ++dit) {
        
        if (dit->second->match(&env))
        {
            poco_debug(rulelog, "determinator " + dit->second->getGUID() + "matched");
            dit->second->execute(&env);
        }
    }
   
    detLock.unlock();
}


void XPLRuleManager::saveDeterminators()
{
    detLock.readLock();
    int ret = 0;
    
    //Path loadLocation(saveLocation);
    Path loadLocation = XPLHal::getConfigFileLocation();
    loadLocation.pushDirectory("determinators");
    
    File loadFile(loadLocation.parent());
    loadFile.createDirectories();
    if (!loadLocation.isDirectory()) {
        poco_error(rulelog, "No directory to save determinators to " + loadLocation.getFileName());
        return;
    }
    
    //delete all current .xml files
    DirectoryIterator it(loadLocation);
    DirectoryIterator end;
    while (it != end)
    {
        Path p(it.path());
        File f(p);
        if(p.isFile() && f.canWrite() && (p.getExtension() == "xml")){
            f.remove();
        }
        ++it;
    }
    
    if (ret == 0){
        for(map<string,Determinator*>::iterator dit = determinators->begin(); dit!=determinators->end(); ++dit) {
            string savename = dit->first;
            if (savename==""){
                continue;
            }
            trimInPlace(replaceInPlace(savename," ", "_"));
            File detFile = ((loadLocation.getFileName()  + savename + ".xml"));
            detFile.createFile();
            FileOutputStream detStream (detFile.path());
            
            //myfile.open (detFile.path().c_str());
            poco_notice(rulelog, "saving determinators to" + detFile.path());
            detStream << (dit->second->printXML());
            detStream.close();
        }
        poco_debug(rulelog, "Saved " + NumberFormatter::format(determinators->size()) + " determinators");
        
    } else {
        poco_error(rulelog, "Cannot save determinators");
    }
    detLock.unlock();
}
void XPLRuleManager::loadDeterminators( ) {
    DIR *dir;
    struct dirent *ent;
    //string loadLocation = saveLocation + "bk";
    
    Path loadLocation = XPLHal::getConfigFileLocation();
    loadLocation.pushDirectory("determinators");
    
    File loadLocationDir(loadLocation);
    loadLocationDir.createDirectories();
//     string loadLocation = saveLocation;
//     dir = opendir ((loadLocation + "/").c_str());
    
    if (!loadLocation.isDirectory()) {
        poco_error(rulelog, "No directory to load determinators from: " + loadLocation.getFileName());
        return;
    }
    
    DirectoryIterator it(loadLocation);
    DirectoryIterator end;
    while (it != end)
    {
        poco_information(rulelog, "reading determinator from " + it.name());
        
        Path p(it.path());
        File f(p);
        if(p.isFile() && f.canRead() && (p.getExtension() == "xml")){
            
             FileInputStream detFile(f.path());
             
          
            std::ifstream myfile;
            std::string detstr;
            
            detstr.resize ( f.getSize() );
            detFile.read ( &detstr[0], detstr.size() );
            Determinator* d = new Determinator ( detstr );
            detLock.writeLock();
            (*determinators)[d->getGUID()] = d;
            detLock.unlock();
            detFile.close();
        }
        ++it;
    }
    poco_debug(rulelog, "loaded " + NumberFormatter::format(determinators->size()) + " determinators");
//     cout << "loaded " << determinators->size() << " determinators\n";
}


