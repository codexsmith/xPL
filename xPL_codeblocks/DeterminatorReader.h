#ifndef DeterminatorReader_H
#define DeterminatorReader_H

#include <string>

using namespace std;

class DeterminatorReader {
	public:
		DeterminatorReader();
		~DeterminatorReader();
		string readDeterminator(string xmlFile);

	private:
		DeterminatorReader(const DeterminatorReader*);
		void operator=(const DeterminatorReader*);
};

#endif //DeterminatorReader_H

