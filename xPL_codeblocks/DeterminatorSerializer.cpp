#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <errno.h>

#include "DeterminatorSerializer.h"
#include "Determinator.h"
#include "DeterminatorFactory.h"
#include "pugixml/pugixml.hpp"
#include "XPLAction.h"

using namespace std;

int DeterminatorSerializer::currentLine_;


DeterminatorSerializer::~DeterminatorSerializer()
{
}

DeterminatorSerializer::DeterminatorSerializer(string xmlFile)
{
    xmlFile_ = xmlFile;
    currentLine_ = 0; //static
}

string DeterminatorSerializer::transformDeterminatorForWriting(Determinator* determinator)
{

}

void DeterminatorSerializer::setXmlFile(char* xmlFile)
{
    xmlFile_ = xmlFile;
}

int DeterminatorSerializer::writeDeterminator(char* xmlString)
{
    int success = 0;
    return success;
}

//takes an entire determinator, as a single \n delimited string
int DeterminatorSerializer::write(char* xmlString)
{
    fstream ruleFile;
    string line;
    int lineCheck;
    string openingLine = "<xplDeterminator>";

    ruleFile.open (xmlFile_.c_str(), ios::out | ios::app); //APPEND!

    if (ruleFile.is_open() and ruleFile.good())
    {
        getline (ruleFile,line);

        lineCheck = line.compare(0,openingLine.length(),openingLine,0,openingLine.length()); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator tag
            ruleFile.write(xmlString,strlen(xmlString)); //APPENDS to the end
            return 0;
        }
    }
    return 1;
}

Determinator* DeterminatorSerializer::parseDeterminator(string xmlString)
{

    size_t size = sizeof(xmlString);
    pugi::xml_document doc;

    Determinator* outRule;
    DeterminatorFactory factory;

    vector<string> conditionsIn = vector<string>();
    XPLAction* action;
    XPLCondition* condition;
    vector<XPLMessage> tmpMsgsIn = vector<XPLMessage>();

    string tmpStr;
    char* tmpChr;
    XPLMessage* tmpMsg;


    pugi::xml_parse_result result = doc.load_buffer(xmlString.c_str(), size);

    pugi::xml_node root = doc.document_element();//root

    pugi::xml_node conditions = root.child("xplCondition");
    pugi::xml_node actions = root.child("xplAction");
    pugi::xml_node parameters;
    string msg_type, srcAddress, dstAddress, schema;
    int hops;
    vector<string> parametersIn = vector<string>();

    for (pugi::xml_attribute_iterator ait = conditions.attributes_begin(); ait != conditions.attributes_end(); ++ait)
    {
        strcat(tmpChr,ait->name());
        strcat(tmpChr,"=");
        strcat(tmpChr, ait->value());
        tmpStr = string(tmpChr);
        conditionsIn.push_back(tmpStr);
    }

    condition = factory.createXPLCondition(conditionsIn);

    msg_type = string(actions.attribute("msg_type").value());
    dstAddress = string(actions.attribute("msg_target").value());
    schema = string(actions.attribute("msg_schema").value());
    hops = factory.HOPS;

    parameters = actions.child("param");

    for (pugi::xml_attribute_iterator ait = parameters.attributes_begin(); ait != parameters.attributes_end(); ++ait)
    {
        strcat(tmpChr, ait->name());
        strcat(tmpChr, "=");
        strcat(tmpChr, ait ->value());
        tmpStr = string(tmpChr);
        parametersIn.push_back(tmpStr);
    }
    //does this need to be in a loop? can we always assume that there will be one action message per determinator? i dont think so.
    tmpMsg = factory.createXPLMessage(msg_type, srcAddress, dstAddress, schema, hops, &parametersIn);

    tmpMsgsIn.push_back(*tmpMsg);

    action = factory.createXPLAction(&tmpMsgsIn);

    outRule = factory.createDeterminator(condition, action, true);

    return outRule;
}


Determinator* DeterminatorSerializer::readDeterminator(string xmlstring)
{
    return parseDeterminator(xmlstring);
}


string DeterminatorSerializer::readFile()
{
    fstream ruleFile;
    string line, lineOut, lineTmp;
    string openingLine = "<xplDeterminator>";
    string endLine = "</xplDeterminator>";
    int lineCheck;
    bool end = false;

    ruleFile.open (xmlFile_.c_str(), ios::in);

    if (ruleFile.is_open() and ruleFile.good())
    {
        getline (ruleFile,line);
        lineOut.append(line);

        lineCheck = line.compare(0,openingLine.length(),openingLine,0,openingLine.length()); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator file
            while(!end and ruleFile.good())
            {
                lineTmp = read();
                lineOut.append(lineTmp);

                lineCheck = lineTmp.compare(0,endLine.length(),endLine,0,endLine.length());
                if (lineCheck == 0){
                    end = true;
                }
            }
        }//end of determinator file
        else
        {
             perror ("Error opening file, not formatted correctly. First line is not an xPLDeterminator tag. %s\n");
        }

        currentLine_ = ruleFile.tellg();
        ruleFile.flush();
        ruleFile.close();
    }
    return lineOut;
}

//returns an entire determinator, with newline characters
string DeterminatorSerializer::read()
{

    fstream ruleFile;
    string line, lineOut, lineTmp;
    string openingLine = "<xplDeterminator>";
    string checkLine = "<determinator>";
    string stopLine = "</determinator>";
    int lineCheck;
    bool end = false;

    ruleFile.open (xmlFile_.c_str(), fstream::in);

    if (currentLine_ != 0)
    {
        ruleFile.seekg(currentLine_);
    }

    if (ruleFile.is_open() and ruleFile.good())
    {

        getline (ruleFile,line);


        lineCheck = line.compare(0,openingLine.length(),openingLine,0,openingLine.length()); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator file tag
            cout << "xplRule";

            getline(ruleFile,line);
            lineOut.append(line);
            lineCheck = line.compare(0,checkLine.length(),checkLine,0,checkLine.length());

            if (lineCheck == 0){//this is an opening determinator tag
                cout << "rule";
                while ( ruleFile.good() and !end)
                {

                    getline(ruleFile, lineTmp);

                    lineOut.append(lineTmp+"\n"); //adding back in newline delimeters, since getline removes them

                    lineCheck = lineTmp.compare(0,stopLine.length(),stopLine,0,stopLine.length());
                    if (lineCheck == 0){
                        end = true; //end of a determinator
                    }
                }
            }
        }
        else
        {
             perror ("Error opening file, not formatted correctly. First line is not a determinator tag.: %s\n");
        }

        currentLine_ = ruleFile.tellg();
        ruleFile.flush();
        ruleFile.close();

    }
    else cout << "Unable to open file";

    return lineOut;
}
