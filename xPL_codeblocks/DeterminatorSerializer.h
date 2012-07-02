#ifndef DeterminatorSeriallizer_H
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
		Determinator read();
		void write(Determinator determinator);

	private:
		DeterminatorSerializer(const DeterminatorSerializer&);
		void operator=(const DeterminatorSerializer&);
		string xmlFile_;
		DeterminatorReader reader_;
		DeterminatorWriter writer_;
};

#endif //DeterminatorSerializer_H
