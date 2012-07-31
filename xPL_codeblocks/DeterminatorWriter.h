#ifndef DeterminatorWriter_H
#define	DeterminatorWriter_H

#include <string>

using namespace std;

class DeterminatorWriter {
	public:
		DeterminatorWriter();
		~DeterminatorWriter();
		void writeDeterminator(string xmlString, string xmlFile);

	private:
		DeterminatorWriter(const DeterminatorWriter*);
		void operator=(const DeterminatorWriter*);
};

#endif //DeterminatorWriter_H

