#ifndef DeterminatorCondition_H
#define DeterminatorCondition_H

#include "XPLCommon.h"

#include "XPLMessage.h"
#include <string>
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"

using namespace std;
class DeterminatorCondition {
	public:
		DeterminatorCondition() {};
		~DeterminatorCondition() {};
    virtual bool match(DeterminatorEnvironment*) = 0;
		//bool equals(DeterminatorCondition* condition);
		virtual void appendCondition( pugi::xml_node* inputnode ) = 0;

	protected:
		
	private:
		string display_name;
		
};

#endif //DeterminatorCondition_H
