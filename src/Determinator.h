#ifndef Determinator_H
#define Determinator_H

#include "DeterminatorAction.h"
#include "XPLAction.h"
#include "XPLCondition.h"
#include "xplMsg.h"
#include "DeterminatorEnvironment.h"

#include <vector>

using namespace std;

class Determinator {
public:
    explicit Determinator();
    explicit Determinator( XPLCondition* condition, DeterminatorAction* action );
    explicit Determinator( string );
    ~Determinator();
    bool match( DeterminatorEnvironment* env );
    void execute(DeterminatorEnvironment*);
    void setEnabled(bool enable);
    bool isEnabled();
//     void setAction(DeterminatorAction* action);
    string printXML();
    void setGUID(string GUID);
    string getGUID();
    string name;
    string description;
    vector<DeterminatorAction*> actions;
    vector<DeterminatorCondition*> conditions;
    bool matchany;
//     DeterminatorAction* getAction();

private:
    string GUID_;

//     DeterminatorAction* action_;
    bool enabled_;
};

#endif //Determinator_H