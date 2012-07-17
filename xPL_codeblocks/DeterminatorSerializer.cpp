#include <iostream>
#include <fstream>
#include <string>
#include "DeterminatorSerializer.h"
#include "Determinator.h"

using namespace std;

DeterminatorSerializer::DeterminatorSerializer(string xmlFile)
{
    xmlFile_ = xmlFile;

}

void DeterminatorSerializer::writeDeterminator(string xmlString)
{}

Determinator* DeterminatorSerializer::write()
{}

Determinator* DeterminatorSerializer::readDeterminator()
{
    string line;
    string openingLine = "<determinator";
    string endLine = "</determinator>";
    fstream ruleFile;
    int lineCheck;
    bool end = false;


    ruleFile.open ("example.txt", ios::in | ios::out);

    if (ruleFile.is_open())
    {

    }
}


string DeterminatorSerializer::read()
{
    string line;
    string openingLine = "<determinator";
    string endLine = "</determinator>";
    fstream ruleFile;
    int lineCheck;
    bool end = false;


    ruleFile.open ("example.txt", ios::in | ios::out);

    if (ruleFile.is_open())
    {
        while ( ruleFile.good() )
        {
            getline (ruleFile,line);
            lineCheck = line.compare(0,13,openingLine,0,13);
            if (lineCheck == 0){
                while(!end){
                    //read the determinator value pairs

                    lineCheck = line.compare(0,15,endLine,0,15);
                    if (lineCheck == 0){
                        end = true; //end of a determinator
                    }
                }
            }
        ruleFile.close();
        }
    }
      else cout << "Unable to open file";
    return line;
}
