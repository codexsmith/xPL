#include "Determinator.h"
#include "conditions/XPLCondition.h"
#include "conditions/TimeCondition.h"
#include "conditions/DayCondition.h"
#include "conditions/GlobalCondition.h"
#include "conditions/GlobalChangedCondition.h"

#include "actions/DeterminatorAction.h"
#include "actions/XPLAction.h"
#include "actions/GlobalAction.h"
#include "actions/LogAction.h"

#include "Poco/UUIDGenerator.h"
#include "Poco/String.h"

#include "Poco/Logger.h"
#include "Poco/NumberFormatter.h"

#include <vector>
#include <iostream>
#include <string.h>

using namespace Poco;

//default constructor
Determinator::Determinator() :
    detlog ( Logger::get ( "rulemanager.determinator" ) )
{
    //cout << "create determinator " << this << " \n";
}

//Determinators should be constructed by the DeterminatorFactory only.
//User must have already created the appropriate condition and action.
//New Determinators are always enabled.
Determinator::Determinator ( XPLCondition* condition, DeterminatorAction* action ) :
    detlog ( Logger::get ( "rulemanager.determinator" ) )
{
    //cout << "create determinator from args\n";
    //condition_ = condition;
    conditions.push_back ( condition );
// 	action_ = action;
    actions.push_back ( action );
    enabled_ = true;
}

Determinator::Determinator ( string  detin ) :
    detlog ( Logger::get ( "rulemanager.determinator" ) )
{

    poco_debug ( detlog, "Creating determinator from xml." );
    //cout << "create determinator from xml: " << this << " \n" ;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load ( detin.c_str(), detin.length() );
    if ( result )
    {
//         std::cout << "XML [" <<  "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
    }
    else
    {
        poco_error ( detlog, "Determinator had parse errors: " );
        poco_error ( detlog, "Error description: " + string ( result.description() ) );
        //poco_error(detlog, "Error offset: " + result.offset + " (error at [..." + (detin.c_str() + result.offset) + "]");

        //std::cout << "XML [" <<  "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
//         std::cout << "Error description: " << result.description() << "\n";
//         std::cout << "Error offset: " << result.offset << " (error at [..." << (detin.c_str() + result.offset) << "]\n\n";
    }
//     cout << "parsing det\n";


    pugi::xml_node detnode;
    //we need to orient ourselves within this block of XML
    if ( doc.child ( "xplDeterminator" ) && doc.child ( "xplDeterminator" ).child ( "determinator" ) )
    {
        detnode = doc.child ( "xplDeterminator" ).child ( "determinator" );
    }
    else if ( doc.child ( "determinator" ) )
    {
        detnode = doc.child ( "determinator" );
    }
    else
    {
        poco_error ( detlog, "Determinator XML is empty" );
        return;
    }


//     cout << "detnode " << detnode.text() << "\n";

    bool failed = false;

    if ( detnode.attribute ( "guid" ) )
    {
        GUID_ = cleanGUID ( detnode.attribute ( "guid" ).as_string() );
    }
    else
    {
        //this determinator may not have one yet, we just make one up
        GUID_ = cleanGUID ( UUIDGenerator::defaultGenerator().createOne().toString() );
    }
    if ( detnode.attribute ( "name" ) )
    {
        name = detnode.attribute ( "name" ).as_string();
    }
    else
    {
        failed = true;
    }
    if ( detnode.attribute ( "description" ) )
    {
        description = detnode.attribute ( "description" ).as_string();
    }
    else
    {
        failed = true;
    }
    if ( detnode.attribute ( "enabled" ) )
    {
        if ( !strcmp ( detnode.attribute ( "enabled" ).as_string(), "Y" ) ) enabled_=true;
        else enabled_ = false;
    }
    else
    {
        failed = true;
    }
    if ( detnode.attribute ( "isGroup" ) )
    {
        if ( !strcmp ( detnode.attribute ( "isGroup" ).as_string(), "Y" ) ) isGroup=true;
        else isGroup = false;
    }
    if ( detnode.attribute ( "groupName" ) )
    {
        groupName = detnode.attribute ( "groupName" ).as_string();
    }

    if ( detnode.child ( "input" ) )
    {
        pugi::xml_node innode =detnode.child ( "input" );

        if ( innode.attribute ( "match" ) )
        {
            if ( !strcmp ( innode.attribute ( "match" ).as_string(), "any" ) ) matchany=true;
            else matchany = false;
        }
        else
        {
            failed = true;
        }

        for ( pugi::xml_node_iterator ait = innode.begin(); ait != innode.end(); ++ait )
        {
            if ( !strcmp ( "xplCondition",ait->name() ) )
            {
                conditions.push_back ( new XPLCondition ( *ait ) );
            }
            else if ( !strcmp ( "timeCondition",ait->name() ) )
            {
                conditions.push_back ( new TimeCondition ( *ait ) );
            }
            else if ( !strcmp ( "dayCondition",ait->name() ) )
            {
                conditions.push_back ( new DayCondition ( *ait ) );
            }
            else if ( !strcmp ( "globalCondition",ait->name() ) )
            {
                conditions.push_back ( new GlobalCondition ( *ait ) );
            }
            else if ( !strcmp ( "globalChanged",ait->name() ) )
            {
                conditions.push_back ( new GlobalChangedCondition ( *ait ) );
            }
        }
        //cout << "\tloaded " << actions.size() << " conditions\n";
    }

    if ( detnode.child ( "output" ) )
    {
        pugi::xml_node outnode =detnode.child ( "output" );
        for ( pugi::xml_node_iterator ait = outnode.begin(); ait != outnode.end(); ++ait )
        {
            if ( !strcmp ( "xplAction",ait->name() ) )
            {
                XPLAction* xa = new XPLAction ( *ait );
                actions.push_back ( xa );
            }
            if ( !strcmp ( "logAction",ait->name() ) )
            {
                actions.push_back ( new LogAction ( *ait ) );
            }
            if ( !strcmp ( "globalAction",ait->name() ) )
            {
                actions.push_back ( new GlobalAction ( *ait ) );
            }
        }
        poco_information ( detlog, "Loaded " + NumberFormatter::format ( actions.size() ) + " actions" );
    }



}

Determinator::~Determinator()
{
    /*
      cout << actions.size() << " actions to delete\n";*/
    while ( actions.size() > 0 )
    {
        DeterminatorAction* de = actions.back();
        actions.pop_back();
        //cout << "want to delete action: " << de << "\n";
        delete ( de );
    }
    while ( conditions.size() > 0 )
    {
        DeterminatorCondition* de = conditions.back();
        conditions.pop_back();
        //cout << "want to delete condition: " << de << "\n";
        delete ( de );
    }

//   cout << "delete determinator: " << this << " \n";

}


string Determinator::getGUID()
{
    return GUID_;
}

void Determinator::setGUID ( string GUID )
{
    GUID_ = cleanGUID ( GUID );
}

//Passthrough to XPLCondition to test if the message matches.
//Always fails when the Determinator is disabled.
bool Determinator::match ( DeterminatorEnvironment* env )
{

    bool result = false;
    if ( matchany )
    {
        poco_trace ( detlog, "Match Any" );
        result =false;
    }
    else
    {
        poco_trace ( detlog, "Match All" );
        result = true;
    }


    if ( enabled_ )
    {
        for ( vector<DeterminatorCondition*>::iterator dit = conditions.begin(); dit != conditions.end(); ++dit )
        {
            if ( matchany )
            {
                result |= ( *dit )->match ( env );
            }
            else
            {
                result &= ( *dit )->match ( env );
                //early exit
                if ( !result ) return false;
            }
        }
        return result;
    }
    return false;
}

//pass through to XPLActions.
//TODO: we need to handle order.
void Determinator::execute ( DeterminatorEnvironment* env )
{
    if ( isEnabled() )
    {
        poco_notice ( detlog, "executing determinator " + name + " : " + description );
        //we sort them by executeOrder before running them
        std::sort(actions.begin(),actions.end());
        for ( vector<DeterminatorAction*>::iterator dit = actions.begin(); dit != actions.end(); ++dit )
        {
            ( *dit )->execute ( env );
            flush ( cout );
        }
    }
}

//Disabled determinators can never be executed. They are on by default.
void Determinator::setEnabled ( bool enable )
{
    enabled_ = enable;
}

//Tests if this determinator can be executed or matched.
bool Determinator::isEnabled()
{
    return enabled_;
}


struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    virtual void write ( const void* data, size_t size )
    {
        result += std::string ( static_cast<const char*> ( data ), size );
    }
};


//Turns Determinator and its collaborators into formatted XML
//for serialization.
string Determinator::printXML()
{

    pugi::xml_document doc;
    //pugi::xml_node descr = doc.document_element().append_child("description");
    // add node with some name
    pugi::xml_node det = doc.append_child ( "determinator" );
    det.append_attribute ( "guid" ) = GUID_.c_str();
    det.append_attribute ( "name" ) = name.c_str();
    det.append_attribute ( "description" ) = description.c_str();
    if ( enabled_ )
    {
        det.append_attribute ( "enabled" ) = "Y";
    }
    else
    {
        det.append_attribute ( "enabled" ) = "N";
    }
    det.append_attribute ( "groupName" ) = "";
    det.append_attribute ( "isGroup" ) = "N";


    pugi::xml_node inputnode = det.append_child ( "input" );
    if ( matchany )
    {
        inputnode.append_attribute ( "match" ) = "any";
    }
    else
    {
        inputnode.append_attribute ( "match" ) = "all";
    }

    for ( vector<DeterminatorCondition*>::iterator dit = conditions.begin(); dit != conditions.end(); ++dit )
    {

        ( *dit )->appendCondition ( &inputnode );
    }
    //condition_->appendCondition(&inputnode);

    pugi::xml_node outputnode = det.append_child ( "output" );

    for ( vector<DeterminatorAction*>::iterator dit = actions.begin(); dit != actions.end(); ++dit )
    {

        ( *dit )->appendAction ( &outputnode );
    }


    xml_string_writer xmlwrite;

    doc.save ( xmlwrite );
    return xmlwrite.result;

}

string Determinator::cleanGUID ( const string guidin )
{
    return toLower ( guidin );
}

string Determinator::unescape ( const string input )
{
    string ret =input;
    replaceInPlace ( ret, "&lt;","<" );
    replaceInPlace ( ret, "&gt;",">" );
    return ret;
}

