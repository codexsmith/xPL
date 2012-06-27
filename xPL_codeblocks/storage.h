#include <string>
#include <vector>
#include "XPLHal.h"

class XPLCondition {
	vector<XPLValuePair> attributes;

	public:
		XPLCondition(vector<XPLValuePair> attributes);
		bool match(XPLMessage);
};

class XPLAction {
	vector<XPLMessage> responses;

	public:
		XPLAction(vector<XPLMessage> responses);
		vector<XPLMessage> execute();
};

class Determinator {
	XPLCondition condition;
	XPLAction action;
	bool enabled;

	public:
		Determinator(XPLCondition condition, XPLAction action);
		bool match(XPLMessage message);
		vector<XPLMessage> execute();	
		void setEnabled(bool status);
		bool isEnabled();
};

class DeterminatorSerializer {
	string xmlFile;

	public:
		DeterminatorSerializer(string xmlFile);
		Determinator read();
		void write(Determinator determinator);
};

class DeterminatorReader {
	public:
		string readDeterminator(string xmlFile);
};

class DeterminatorWriter {
	public:
		void writeDeterminator(string xmlString, string xmlFile);
};
