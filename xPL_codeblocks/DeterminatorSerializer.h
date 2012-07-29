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
		string read();
		Determinator* readDeterminator();
		int writeDeterminator(char* xmlString);
        int write(char* xmlString);

	private:
		char* xmlFile_;
        static int currentLine_;

};

#endif //DeterminatorSerializer_H
