#include <iostream>
#include <fstream>
#include <errno>
#include <string>
#include "DeterminatorSerializer.h"
#include "Determinator.h"

using namespace std;

DeterminatorSerializer::DeterminatorSerializer(string xmlFile)
{
    xmlFile_ = xmlFile;
    currentLine_ = 0; //static

}

void DeterminatorSerializer::writeDeterminator(string xmlString)
{

}

//takes an entire determinator, as a single \n delimited string
Determinator* DeterminatorSerializer::write(string xmlString)
{

    fstream ruleFile;
    string line;
    ruleFile.open (xmlFile_, ios::in);

    if (ruleFile.is_open() and ruleFile.good())
    {
        getline (ruleFile,line);

        lineCheck = line.compare(0,13,openingLine,0,13); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator tag

        }
    }

}

Determinator* DeterminatorSerializer::readDeterminator()
{
    string determinatorIn;
    string openingLine = "<determinator";
    string endLine = "</determinator>";

    fstream ruleFile;

    int lineCheck;
    bool end = false;

    determinatorIn = read();

    //make determinator & return


}

//returns an entire determinator, with newline characters
string DeterminatorSerializer::read()
{
    string line, lineOut, lineTmp;
    string openingLine = "<determinator";
    string endLine = "</determinator>";
    fstream ruleFile;
    int lineCheck;
    bool end = false;

    ruleFile.open (xmlFile_, ios::in);

    ruleFile.seekg(currentLine_);

    if (ruleFile.is_open() and ruleFile.good())
    {
        getline (ruleFile,line);
        lineOut.append(line);

        lineCheck = line.compare(0,13,openingLine,0,13); //is this a proper opening line?

        if (lineCheck == 0)
        { //yes this is an opening determinator tag

            while ( ruleFile.good() and !end)
            {
                getline(ruleFile, lineTmp);

                lineOut.append(lineTmp+"\n"); //adding back in newline delimeters, since getline removes them

                lineCheck = lineTmp.compare(0,15,endLine,0,15);
                if (lineCheck == 0){
                    end = true; //end of a determinator
                }
            }
            else
            {
                 printf ("Error opening file, not formatted correctly. First line is not a determinator tag.: %s\n",strerror(errno));
            }
        }
        currentLine_ = ruleFile.tellg();
        ruleFile.flush();
        ruleFile.close();

    }
    else cout << "Unable to open file";

    return lineOut;
}
