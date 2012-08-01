#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <string>
#include "DeterminatorSerializer.h"
#include "Determinator.h"
#include "DeterminatorFactory.h"
#include "pugixml/pugixml.hpp"
#include "XMLpugiXPL.h"
#include "XPLAction.h"

using namespace std;

int DeterminatorSerializer::currentLine_;

DeterminatorSerializer::~DeterminatorSerializer()
{
}

DeterminatorSerializer::DeterminatorSerializer(char* xmlFile)
{
    xmlFile_ = xmlFile;
    currentLine_ = 0; //static
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

    ruleFile.open (xmlFile_, ios::out | ios::app); //APPEND!

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

Determinator* DeterminatorSerializer::readDeterminator()
{
    string determinatorString = read();
    vector<XPLMessage>* messages = new vector<XPLMessage>();
    vector<string>* attributes = new vector<string>();

    XPLAction* action = DeterminatorFactory::createXPLAction(messages);
    XPLCondition* condition = DeterminatorFactory::createXPLCondition(*attributes);
    Determinator* determinator = DeterminatorFactory::createDeterminator(condition, action, true);

    //make determinator & return

    return NULL;
}

string DeterminatorSerializer::readFile()
{
    fstream ruleFile;
    string line, lineOut, lineTmp;
    string openingLine = "<xplDeterminator>";
    string endLine = "</xplDeterminator>";
    int lineCheck;
    bool end = false;

    ruleFile.open (xmlFile_, ios::in);

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
             perror ("Error opening file, not formatted correctly. First line is not a xplDeterminator tag. %s\n");
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
    string endLine = "</xplDeterminator>";
    int lineCheck;
    bool end = false;

    ruleFile.open (xmlFile_, fstream::in);

    if (currentLine_ != 0)
    {
        ruleFile.seekg(currentLine_);
    }
    cout << "before";
    if (ruleFile.is_open() and ruleFile.good())
    {
        cout << "rulefile is open & good";
        getline (ruleFile,line);
        lineOut.append(line);

        lineCheck = line.compare(0,openingLine.length(),openingLine,0,openingLine.length()); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator tag

            while ( ruleFile.good() and !end)
            {
                getline(ruleFile, lineTmp);

                lineOut.append(lineTmp+"\n"); //adding back in newline delimeters, since getline removes them

                lineCheck = lineTmp.compare(0,endLine.length(),endLine,0,endLine.length());
                if (lineCheck == 0){
                    end = true; //end of a determinator
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
    else cout << "Unable to open fileDS";

    return lineOut;
}
