#ifndef DeterminatorSeriallizer_H
#define DeterminatorSerializer_H

#include <string>
#include "Determinator.h"

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
};

#endif //DeterminatorSerializer.h
