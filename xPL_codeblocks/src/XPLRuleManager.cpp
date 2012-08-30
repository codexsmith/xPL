#include "XPLRuleManager.h"
#include <vector>
#include <fstream>


#include <syslog.h>

#include <sys/types.h>
#include <sys/stat.h>
//#ifdef HAVE_UNISTD_H
#include <unistd.h>
//#endif /* HAVE_UNISTD_H */
//#include <string.h>
//#include "sysstat.h"    /* Fix up for Windows - inc mode_t */
#include <dirent.h>

using namespace std;

const string XPLRuleManager::saveLocation = "/tmp/determinators";

XPLRuleManager::XPLRuleManager(vector<Determinator>* determinators)
{
    //loadDeterminators();
    this->determinators = determinators;
    //this->determinators = loadDeterminators();
}


XPLRuleManager::XPLRuleManager()
{
    //this->determinators = new vector<Determinator>();
    this->determinators = loadDeterminators();
}

XPLRuleManager::~XPLRuleManager()
{
    printf("trying to save determinators\n");
    saveDeterminators();
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
    return;
    //match stuff
    for (int i = 0; i < determinators->size(); i++)
    {
        if (determinators->at(i).match(&msg))
        {
            determinators->at(i).execute();
        }
    }
}

int XPLRuleManager::makeDeterminatorDir()
{
    struct stat      st;
    int             status = 0;
    if (stat(saveLocation.c_str(), &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(saveLocation.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }
    
    return(status);
}

void XPLRuleManager::saveDeterminators()
{
    int ret = makeDeterminatorDir();
    std::ofstream myfile;
    
    if (ret == 0){
        for(vector<Determinator>::iterator dit = determinators->begin(); dit!=determinators->end(); ++dit) {
            myfile.open ((saveLocation +"/" +(*dit).getGUID() + ".xml").c_str());
            cout << "Saving " + saveLocation +"/" +(*dit).getGUID() + ".xml\n";
            myfile << ((*dit).printXML()).c_str();
            myfile.close();
        }
        cout << "Saved "<< determinators->size() << " determinators\n";
        flush(cout);
    } else {
        cout << "Cannot save determinators\n";
        flush(cout);
    }
}
vector< Determinator >* XPLRuleManager::loadDeterminators() {
    DIR *dir;
    struct dirent *ent;
    string loadLocation = saveLocation + "bk";
    dir = opendir ((loadLocation + "/").c_str());
    vector<Determinator>*  loaded = new vector<Determinator>();
    
    if (dir != NULL) {
        /* print all the files and directories within directory */
        cout << "determinators:\n";
       
        while ((ent = readdir (dir)) != NULL) {
            std::ifstream myfile;
            std::string detstr;
            string filename = (loadLocation + "/" + ent->d_name);
            string ending = ".xml";
            string ending2 = loadLocation + "/" +".xml";
            if (filename.length() >  ending2.length() && (filename.compare (filename.length() - ending.length(), ending.length(), ending) == 0)) {
                cout << "\tloading: "+  filename + "\n"; 
            
                myfile.open(filename.c_str(),std::ios::in | std::ios::binary);

                if(myfile.bad()) {
                    cout<<"fail\n";
                    return loaded;
                }

                myfile.seekg(0, std::ios::end);
                int leng = myfile.tellg();
                detstr.resize(leng);
                myfile.seekg(0, std::ios::beg);
                myfile.read(&detstr[0], detstr.size());
                
                Determinator* d = new Determinator(detstr);

                loaded->push_back(*d);
                myfile.close();
            }
            
        }
        closedir (dir);

    } else {
        /* could not open directory */
        syslog(LOG_ERR, ("Failed to open determinator directory " + loadLocation).c_str());
        cout << ("Failed to open determinator directory " + loadLocation);
    }
    cout << "loaded " << loaded->size() << " determinators\n";
    return loaded;
}
