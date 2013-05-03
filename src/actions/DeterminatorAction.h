#ifndef DeterminatorAction_H
#define DeterminatorAction_H

#include <vector>
#include <iostream>
#include "pugixml/pugixml.hpp"
#include "DeterminatorEnvironment.h"
using namespace std;

/**
 * @brief An action from the determinator XML. This is an action that will be taken when a determinator rule matches.
 **/
class DeterminatorAction
{
public:

    /**
     * @brief Executes the Action.
     *
     * @param env An environment passed in, used for things like Global replacement.
     * @return void
     **/
    virtual void execute ( DeterminatorEnvironment* env ) = 0;
    /**
     * @brief Outputs this action as Determinator XML
     *
     * @param outputnode The node that a logAction element will be added to, usually the output section
     * @return void
     **/
    virtual void appendAction ( pugi::xml_node* outputnode ) = 0;

    //bool equals(XPLAction* action);

    virtual ~DeterminatorAction() {};
    /**
     * @brief A friendly name for the determinator
     *
     **/
    string display_name;
    /**
     * @brief A number represententing the order that the actions should be executed in within a determinator
     **/
    
    
    /**
     * @brief We define these comparison operators so we can sort a vector of actions
     **/
    bool operator> (DeterminatorAction &other)
    {
        return executeOrder > other.executeOrder;
    }
    
    bool operator< (DeterminatorAction &other)
    {
        return executeOrder < other.executeOrder;
    }
    
    int executeOrder;
};

#endif //DeterminatorAction_H
