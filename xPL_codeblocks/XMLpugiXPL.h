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

        void printDeterminatorXML(string xmlstring);
		Determinator* readDeterminator();

};
#endif //XMLpugiXPL_H
