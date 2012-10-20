#ifndef DeterminatorCondition_H
#define DeterminatorCondition_H

#include "../XPLCommon.h"

#include <string>
#include "../pugixml/pugixml.hpp"
#include <DeterminatorEnvironment.h>

using namespace std;

//this is just here to encapsulate the stuff needed for the operators in many conditions. Oh, what a mess...
class DeterminatorOperator
{
public:
    enum Type
    {
        Equals,NotEquals,GreaterThan,LessThan,GreaterThanOrEquals,LessThanOrEquals
    };
    Type t_;
    DeterminatorOperator(): t_(Equals){};
    DeterminatorOperator(Type t) : t_(t) {};
    DeterminatorOperator(string in) {
        if(in=="=")
            t_ = Equals;
        else if(in=="!=")
            t_ = NotEquals;
        else if(in=="<")
            t_ = LessThan;
        else if(in==">")
            t_ = GreaterThan;
        else if(in=="<=")
            t_ = LessThanOrEquals;
        else if(in==">=")
            t_ = GreaterThanOrEquals;
        else
            t_ = Equals;
    };
    bool operator==(DeterminatorOperator const &rhs) const {
        if(t_ ==rhs.t_) return true;
        return false;
    }
    operator Type () const {return t_;}
    string toString() {
        if (t_ == Equals)
            return "=";
        else if (t_ == NotEquals)
            return "!=";
        else if (t_ == LessThan)
            return "<";
        else if (t_ == GreaterThan)
            return ">";
        else if (t_ == LessThanOrEquals)
            return "<=";
        else if (t_ == GreaterThanOrEquals)
            return ">=";
        return "=";
    }
private:
    //prevent automatic conversion for any other built-in types such as bool, int, etc
//     template<typename T>
//     operator T () const;
};


class DeterminatorCondition {
	public:
      
    
      
		DeterminatorCondition() {};
		virtual ~DeterminatorCondition() {};
    virtual bool match(DeterminatorEnvironment*) = 0;
		//bool equals(DeterminatorCondition* condition);
		virtual void appendCondition( pugi::xml_node* inputnode ) = 0;

	protected:
		
	private:
		string display_name;
		
};

#endif //DeterminatorCondition_H
