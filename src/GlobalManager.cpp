#include "GlobalManager.h"
#include "Poco/String.h"
#include <Poco/File.h>
#include <Poco/Path.h>
#include <vector>
#include <iostream>

#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/DOM/AutoPtr.h" //typedef to Poco::AutoPtr
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeList.h"
#include <Poco/FileStream.h>
#include "Poco/SAX/SAXException.h"

#include "XPLHal.h"

using namespace Poco::XML;

using namespace std;

static string globalFile = "xplhal.xml";

static string globalproperty = "globalVars";


GlobalManager::GlobalManager():
globallog(Logger::get("globalmanager")) {
//     loadGlobals();
}

//copy
GlobalManager& GlobalManager::operator=(const GlobalManager& other) {
    
    if (this != &other) // protect against invalid self-assignment
        {
            
            //make a copy
            cout<< "get lock \n";
            //other.globalLock.lock();
            cout<< "got lock \n";
            cout << "othersize " << other.globalVars.size() << "\n";
            //globalVars = other.getGlobals();
            //globalVars = other.globalVars;
            //other.globalLock.unlock();
            cout<< "rel lock \n";
        }
    return *this;
}

GlobalManager::GlobalManager(const GlobalManager& other):
globallog(Logger::get("globalmanager")) {

    //make a copy
    //other.globalLock.lock();
    //globalVars = other.getGlobals();
    globalVars = other.globalVars;
    //other.globalLock.unlock();
}


GlobalManager::~GlobalManager() {
    /*saveGlobals(); */
}
    

void GlobalManager::loadGlobals() {
    globalLock.lock();
    globalvarpath = XPLHal::getConfigFileLocation();
//     globalvarpath = Path(Path::home());
//     
//     globalvarpath.pushDirectory(".xPL");
//     globalvarpath.pushDirectory("xplhallite");
    globalvarpath.setFileName(globalFile);
//     
    //cout << "path is " << globalvarpath.toString() << std::endl;
    poco_information(globallog, "Loading globals from " + globalvarpath.toString());
    
    File globalvarfile = File(globalvarpath);
    
    if(globalvarfile.exists() && globalvarfile.canRead() && globalvarfile.isFile()) {
        
        poco_debug(globallog, "global files exists " + globalvarfile.path() );
        try {
            DOMParser xmlParser;
            AutoPtr<Document> gDoc = xmlParser.parse(globalvarfile.path());

//             NodeIterator it(gDoc, NodeFilter::SHOW_ALL);
    //         AutoPtr<Node> pNode = it.nextNode();
    //         while (pNode)
    //         {
    // 
    //             std::cout << pNode->nodeName() << ":" << pNode->nodeValue() << std::endl;
    //             pNode = it.nextNode();
    //         }
            
            Element* rootElem = gDoc->documentElement();
            if(rootElem->nodeName() != "xPLHalGlobals") {
                //cout << "something is wrong with the globals root element" << std::endl;
                poco_warning(globallog, "something is wrong with the globals root element");
                return;
            }
            
            NodeIterator it(rootElem, NodeFilter::SHOW_ELEMENT);
            Element* pNode = (Element*) it.nextNode();
            while (pNode)
            {
                poco_debug(globallog, "List of loaded globals:");
                if(pNode->hasAttribute("name") && pNode->hasAttribute("value")) {
                    //std::cout << fromXMLString(pNode->getAttribute("name")) << ":" << fromXMLString(pNode->getAttribute("value")) << std::endl;
                    poco_debug(globallog, fromXMLString(pNode->getAttribute("name")) + ":" + fromXMLString(pNode->getAttribute("value")));
                    globalVars[cleanGlobalName(fromXMLString(pNode->getAttribute("name")))] = fromXMLString(pNode->getAttribute("value"));
                }
                
                pNode = (Element*) it.nextNode();
            }
            
        } catch (Poco::XML::SAXParseException e) {
                globalLock.unlock();
                cout << "XML read error" << std::endl;
                return;
        }
        
    } else {
        poco_notice(globallog, "GLOBAL vars XML files doesn't exist"  );
    }
    globalLock.unlock();
}
    
    
void GlobalManager::saveGlobals() {
    globalLock.lock();
    globalvarpath = Path(Path::home());
    
    globalvarpath.pushDirectory(".xPL");
    globalvarpath.pushDirectory("xplhallite");
    globalvarpath.setFileName("xplhal.xml");
    //globalFile
    
    poco_information(globallog, "Saving globals to " + globalvarpath.toString());
    
    File globalvarfile = File(globalvarpath);
//     cout << "make file"<< std::endl;
//     cout << "exist: " <<globalvarfile.exists() << std::endl;
    if( globalvarfile.exists() && globalvarfile.canWrite()) {
        poco_notice(globallog, "removing previous save file");
        globalvarfile.remove();        
    }
    globalvarfile.createFile();
    
    AutoPtr<Document> pDoc = new Document;
    
    AutoPtr<Element> pRoot = pDoc->createElement("xPLHalGlobals");
    pDoc->appendChild(pRoot);
    map<string,string>::iterator git;
    for(git = globalVars.begin(); git != globalVars.end(); ++git) {
        AutoPtr<Element> pChild1 = pDoc->createElement("global");
        pChild1->setAttribute("name", git->first);
        pChild1->setAttribute("value", git->second);
        pRoot->appendChild(pChild1);
        
    }
    DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(XMLWriter::PRETTY_PRINT);
    writer.writeNode(std::cout, pDoc);
    
    if( globalvarfile.exists() && globalvarfile.canRead() && globalvarfile.isFile()) {
        poco_trace(globallog, "writing globals file");
        FileOutputStream saveFile(globalvarfile.path());
        writer.writeNode(saveFile, pDoc);
    } else {
        poco_warning(globallog, "failed to write globals file");   
    }
    
    globalLock.unlock();
}

string GlobalManager::cleanGlobalName(string name ){
    return toLower(name);
}

bool GlobalManager::hasGlobal(string name) {
    bool ret;
    globalLock.lock();
    ret = globalVars.count(name);
    globalLock.unlock();
    return ret;
}
string GlobalManager::getGlobal(string name){
    string ret = "";
    globalLock.lock();
    if (globalVars.count(name)) {
        ret = globalVars[name];
    }
    globalLock.unlock();
    return ret;   
}


bool GlobalManager::deleteGlobal(string name) {
    name = cleanGlobalName(name);
    bool removed = false;
    globalLock.lock();
    if(globalVars.erase(name) != 0 ) {
        removed = true;
    }
    globalLock.unlock();
    return removed;
}

map<string, string> GlobalManager::getGlobals() const {
    cout << "ommmp " << this<< "   " << &globallog << "\n";
    poco_warning(globallog, "omp p " +  NumberFormatter::format(this) ); 
    globalLock.lock();
    poco_warning(globallog, "other p " +  NumberFormatter::format(&globalVars) ); 
    map<string, string> globalcopy(globalVars);
    poco_warning(globallog, "count " +  NumberFormatter::format(globalcopy.size()) );   
    globalLock.unlock();
    return globalcopy;
}

void GlobalManager::setGlobal(string name, string value) {
    globalLock.lock();
    bool changed = false;
    string realName = cleanGlobalName(name);
    if(globalVars[realName] != value) {
        globalVars[realName] = value;
        changed = true;
    }
    globalLock.unlock();
    
    if(changed){
        poco_warning(globallog, realName + " changed, firing event");   
        DeterminatorEnvironment env(realName);
        XPLHal::instance().ruleMgr->match(env);
    }
    
    
}



