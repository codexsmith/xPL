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
		bool transformDeterminatorForWriting(Determinator* determinator);
		int writeDeterminator(char* xmlString);
        int write(char* xmlString);
        void setXmlFile(char* xmlFile); 

	private:
		string xmlFile_;
        static int currentLine_; 

};

#endif //DeterminatorSerializer_H
