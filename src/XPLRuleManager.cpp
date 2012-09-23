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

using namespace Poco;

using namespace std;

const string XPLRuleManager::saveLocation = "/tmp/determinators/";




XPLRuleManager::XPLRuleManager(map<string, Determinator*>* determinators)
{
    //loadDeterminators();
    this->determinators = determinators;
    //this->determinators = loadDeterminators();
}


XPLRuleManager::XPLRuleManager()
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
    cout << "trying to save determinators\n";
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
    cout << "Rulemgr: modified " << (*determinators)[GUID]->name << "\n";
    detLock.unlock();
}

bool XPLRuleManager::removeDeterminator( string GUID ){
    detLock.writeLock();
    map<string,Determinator*>::iterator dit;
    for (dit = determinators->begin(); dit != determinators->end();) {
        if(dit->first == Determinator::cleanGUID(GUID)) {
            Determinator* detp = dit->second;
            cout <<"f1\n";
            determinators->erase(dit);
            cout <<"f2\n";
            delete detp;
            cout <<"f3\n";
            detLock.unlock();   
            cout <<"f4\n";
            return true;
        }
        cout <<"not it\n";
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
    cout << "Warning: determinator " << GUID << "doesn't exist, so not deleting\n";
    return false;
}
bool XPLRuleManager::runDeterminator( string GUID ){
    detLock.readLock();
    if (determinators->count( GUID ) ==1) {
        Determinator* detp = (*determinators)[GUID];
        DeterminatorEnvironment env;
        detp->execute(&env); //FIXME this is against spec - we're supposed to check the conditions first?
        detLock.unlock();   
        return true;
    }
    detLock.unlock();   
    cout << "Warning: determinator " << GUID << "doesn't exist, so not deleting\n";
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
            //cout << "matched\n";
            dit->second->execute(&env);
        }
    }
   
    detLock.unlock();
}


void XPLRuleManager::saveDeterminators()
{
    detLock.readLock();
    int ret = 0;
    
    
    
    Path loadLocation(saveLocation);
    //     string loadLocation = saveLocation;
    //     dir = opendir ((loadLocation + "/").c_str());
    loadLocation.makeDirectory();
    if (!loadLocation.isDirectory()) {
        cout << "no such dir\n";
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
            File detFile = ((saveLocation  + savename + ".xml"));
            detFile.createFile();
            FileOutputStream detStream (detFile.path());
            
            //myfile.open (detFile.path().c_str());
            cout << "Saving " + detFile.path() <<"\n";
            detStream << (dit->second->printXML());
            detStream.close();
        }
        cout << "Saved "<< determinators->size() << " determinators\n";
        flush(cout);
    } else {
        cout << "Cannot save determinators\n";
        flush(cout);
    }
    detLock.unlock();
}
void XPLRuleManager::loadDeterminators( ) {
    DIR *dir;
    struct dirent *ent;
    //string loadLocation = saveLocation + "bk";
    
    Path loadLocation(saveLocation);
    File loadLocationDir(loadLocation);
    loadLocationDir.createDirectory();
//     string loadLocation = saveLocation;
//     dir = opendir ((loadLocation + "/").c_str());
    
    if (!loadLocation.isDirectory()) {
        cout << "no such dir\n";
        return;
    }
    
    DirectoryIterator it(loadLocation);
    DirectoryIterator end;
    while (it != end)
    {
        std::cout << it.name();
       
        
        Path p(it.path());
        File f(p);
        if(p.isFile() && f.canRead() && (p.getExtension() == "xml")){
            std::cout << f.getSize();
            
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
        std::cout << std::endl;
        ++it;
    }

    cout << "loaded " << determinators->size() << " determinators\n";
}


