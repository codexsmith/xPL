#ifndef DeterminatorSerializer_H
#define DeterminatorSerializer_H

#include <string>
#include "Determinator.h"

using namespace std;

class DeterminatorSerializer {

	public:
		explicit DeterminatorSerializer(string xmlFile);
		~DeterminatorSerializer();

		string readFile();
        string read();

        Determinator* parseDeterminator(string xmlstring);
		Determinator* readDeterminator(string xmlstring);
		string transformDeterminatorForWriting(Determinator* determinator);
		int writeDeterminator(char* xmlString);
        bool write(char* xmlString);
        void setXmlFile(char* xmlFile); 

	private:
		string xmlFile_;
        static int currentLine_; 

};

#endif //DeterminatorSerializer_H
