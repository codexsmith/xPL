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
		string read();
		Determinator* readDeterminator();
		void writeDeterminator(string xmlString);
        Determinator* write();

	private:
		string xmlFile_;

};

#endif //DeterminatorSerializer_H
