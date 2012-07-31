#ifndef DeterminatorSerializer_H
#define DeterminatorSerializer_H

#include <string>
#include "Determinator.h"
#include "DeterminatorWriter.h"
#include "DeterminatorReader.h"

using namespace std;

class DeterminatorSerializer {
	public:
		explicit DeterminatorSerializer(char* xmlFile);
		~DeterminatorSerializer();
		string readFile();
        string read();

		Determinator* readDeterminator();
		bool transformDeterminatorForWriting(Determinator* determinator);
		int writeDeterminator(char* xmlString);
        int write(char* xmlString);
        void setXmlFile(char* xmlFile); //does c++ have a path library?

	private:
		char* xmlFile_;
        static int currentLine_; //here be dragons

};

#endif //DeterminatorSerializer_H
