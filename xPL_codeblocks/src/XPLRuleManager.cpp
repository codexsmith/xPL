#include "XPLRuleManager.h"
#include <vector>
#include <fstream>
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

using namespace Poco;

using namespace std;

const string XPLRuleManager::saveLocation = "/tmp/determinators/";




XPLRuleManager::XPLRuleManager(vector<Determinator*>* determinators)
{
    //loadDeterminators();
    this->determinators = determinators;
    //this->determinators = loadDeterminators();
}


XPLRuleManager::XPLRuleManager()
{
    this->determinators = new vector<Determinator*>();
    loadDeterminators(this->determinators );
    //vector< Determinator >* x = loadDeterminators();
    //this->determinators = x;
    //delete x;
}

XPLRuleManager::~XPLRuleManager()
{
    cout << "trying to save determinators\n";
    saveDeterminators();
//     cout << "delete determinators from  "  << this << " ";
    
    while (determinators->size() > 0 ) {
//         cout << ".";
        delete determinators->back();
        determinators->pop_back();
    }
    
    delete determinators;
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
    for (int i = 0; i < determinators->size();i++){
        //string += determinators -> at(i).getID();
        //string += "\r\n";
    }
    return theString;
}

void XPLRuleManager::match(XPLMessage msg)
{
    cout << "rules engine matching \n";
    //match stuff
    
    DeterminatorEnvironment env = DeterminatorEnvironment(&msg);
    
    for (int i = 0; i < determinators->size(); i++)
    {
        if (determinators->at(i)->match(&env))
        {
            determinators->at(i)->execute(&env);
        }
    }
}


void XPLRuleManager::saveDeterminators()
{

    int ret = 0;
    
    if (ret == 0){
        for(vector<Determinator*>::iterator dit = determinators->begin(); dit!=determinators->end(); ++dit) {

            File detFile = ((saveLocation  +(*dit)->getGUID() + ".xml"));
            //create all parent directories if needed
            detFile.createDirectories();
            FileOutputStream detStream (detFile.path());
            
            //myfile.open (detFile.path().c_str());
            cout << "Saving " + saveLocation  +(*dit)->getGUID() + ".xml\n";
            detStream << ((*dit)->printXML());
            detStream.close();
        }
        cout << "Saved "<< determinators->size() << " determinators\n";
        flush(cout);
    } else {
        cout << "Cannot save determinators\n";
        flush(cout);
    }
}
void XPLRuleManager::loadDeterminators(vector< Determinator*>* loaded) {
    DIR *dir;
    struct dirent *ent;
    //string loadLocation = saveLocation + "bk";
    
    Path loadLocation(saveLocation);
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
            loaded->push_back ( d );
            detFile.close();
        }
        std::cout << std::endl;
        ++it;
    }

    cout << "loaded " << loaded->size() << " determinators\n";
}
