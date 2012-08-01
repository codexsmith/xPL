#ifndef DISPATCHER_H
#define DISPATCHER_H


class Dispatcher{
public:

    Dispatcher(std::string aName){
        name = aName;
    }
    Dispatcher(){

    }
    std::string addRule(std::string aString){
        std::string theString;
        aString += "a";
        return aString;
    }
    std::string listRule(std::string aString){
        std::string theString;
        aString += "b";
        return aString;
    }
    std::string deleteRule(std::string aString){
        return NULL;
    }
    std::string capCommand(std::string aString){
        std::string theString = "236 --010L0\r\n";
        return theString;
    }
private:
    std::string name;
};

#endif
