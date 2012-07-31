#ifndef DeterminatorSerializer_H
#define DeterminatorSerializer_H

#include <string>
#include "Determinator.h"
#include "DeterminatorWriter.h"
#include "DeterminatorReader.h"

using namespace std;

class DeterminatorSerializer {
	public:
		explicit DeterminatorSerializer(string xmlFile);
		~DeterminatorSerializer();
		string readFile();
        string read();
        Determinator* parseDeterminator(string xmlstring);

		Determinator* readDeterminator(string xmlstring);
		int writeDeterminator(char* xmlchar);
        int write(char* xmlchar);
        void setXmlFile(char* xmlFilePath); //does c++ have a path library?

	private:
		string xmlFile_;
        static int currentLine_; //here be dragons

};

#endif //DeterminatorSerializer_H
