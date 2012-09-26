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
using namespace Poco::XML;

using namespace std;

static string globalFile = "xplhal.xml";

static string globalproperty = "globalVars";


GlobalManager::GlobalManager() {
    loadGlobals();
}


GlobalManager::~GlobalManager() {
    saveGlobals();    
}
    

void GlobalManager::loadGlobals() {
    globalLock.lock();
    globalvarpath = Path(Path::home());
    
    globalvarpath.pushDirectory(".xPL");
    globalvarpath.pushDirectory("xplhallite");
    globalvarpath.setFileName(globalFile);
    
    cout << "path is " << globalvarpath.toString() << std::endl;
    
    File globalvarfile = File(globalvarpath);
    cout << "made file"<< std::endl;
    if(globalvarfile.exists() && globalvarfile.canRead() && globalvarfile.isFile()) {
        cout << "global files exists "<< globalvarfile.path() <<  std::endl;
        
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
                cout << "something is wrong with the globals root element" << std::endl;
                
                return;
            }
            
            cout << "list" << std::endl;
            NodeIterator it(rootElem, NodeFilter::SHOW_ELEMENT);
            Element* pNode = (Element*) it.nextNode();
            while (pNode)
            {
                if(pNode->hasAttribute("name") && pNode->hasAttribute("value")) {
                    std::cout << fromXMLString(pNode->getAttribute("name")) << ":" << fromXMLString(pNode->getAttribute("value")) << std::endl;
                    globalVars[cleanGlobalName(fromXMLString(pNode->getAttribute("name")))] = fromXMLString(pNode->getAttribute("value"));
                }
                
                pNode = (Element*) it.nextNode();
            }
            cout << "end of list" << std::endl;
            
        } catch (Poco::XML::SAXParseException e) {
                globalLock.unlock();
                cout << "XML read error" << std::endl;
                return;
        }
        
    } else {
        cout << "GLOBAL vars XML files doesn't exist"<< std::endl;
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
    cout << "path is " << globalvarpath.toString() << std::endl;
    
    File globalvarfile = File(globalvarpath);
    cout << "make file"<< std::endl;
    cout << "exist: " <<globalvarfile.exists() << std::endl;
    if( globalvarfile.exists() && globalvarfile.canWrite()) {
        cout << "globals: removing previous save file" << std::endl;
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
        cout << "globals: can write"<< std::endl;
        
        FileOutputStream saveFile(globalvarfile.path());
        writer.writeNode(saveFile, pDoc);
    }
    
    globalLock.unlock();
}

string GlobalManager::cleanGlobalName(string name ){
    return toLower(name);
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

map<string, string> GlobalManager::getGlobals() {
    globalLock.lock();
    map<string, string> globalcopy(globalVars);
    globalLock.unlock();
    return globalcopy;
}

void GlobalManager::setGlobal(string name, string value) {
    globalLock.lock();
    string realName = cleanGlobalName(name);
    globalVars[realName] = value;
    globalLock.unlock();
}



