#ifndef XMLpugiXPL_H
#define XMLpugiXPL_H

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

using namespace std;

class XMLpugiXPL {
	public:
		explicit XMLpugiXPL(char* xmlFile);
		~XMLpugiXPL();
		string readFile();
        string read();

		Determinator* readDeterminator();
		int writeDeterminator(char* xmlString);
        int write(char* xmlString);
        void setXmlFile(char* xmlFile); //does c++ have a path library?

	private:
		char* xmlFile_;
        static int currentLine_; //here be dragons

};

#endif //XMLpugiXPL_H
