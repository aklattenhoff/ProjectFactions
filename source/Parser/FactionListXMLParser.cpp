#include "FactionListXMLParser.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/FactionListXML.h"
#include "Parser/FactionXMLData.h"
#include "Parser/EntityListXMLCreator.h"
#include "Parser/ActionListXMLCreator.h"
#include "Parser/ProjectileListXMLCreator.h"

FactionListXMLParser::FactionListXMLParser()
{
	_factionListXML = NULL;
	_entityListXMLCreator = NULL;
	_actionListXMLCreator = NULL;
	_projectileListXMLCreator = NULL;
}

FactionListXMLParser::~FactionListXMLParser()
{
	delete _factionListXML;
	delete _entityListXMLCreator;
	delete _actionListXMLCreator;
	delete _projectileListXMLCreator;
}

int FactionListXMLParser::Init()
{
	_factionListXML = new FactionListXML();
	_factionListXML->Init();

	_entityListXMLCreator = new EntityListXMLCreator();
	_entityListXMLCreator->Init();

	_actionListXMLCreator = new ActionListXMLCreator();
	_actionListXMLCreator->Init();

	_projectileListXMLCreator = new ProjectileListXMLCreator();
	_projectileListXMLCreator->Init();

	return 0;
}

int FactionListXMLParser::Parse(std::string fileName)
{
	if (!_factionListXML)
		return -1;

	//TODO: Add lots of error checking here!
	tinyxml2::XMLDocument xmlFactionList;
	xmlFactionList.LoadFile(fileName.c_str());

	tinyxml2::XMLElement* factionElement = NULL;

	if (xmlFactionList.FirstChildElement("FactionList"))
		factionElement = xmlFactionList.FirstChildElement("FactionList")->FirstChildElement("Faction");
	else
		DebugOutputFatalError("No faction tag found in faction list file: %s\n", fileName.c_str());

	while (factionElement)
	{
		_factionListXML->_factionXMLDataVector.push_back(new FactionXMLData());
		_factionListXML->_factionXMLDataVector.back()->Init();

		//First set the file name strings
		_factionListXML->_factionXMLDataVector.back()->_name = factionElement->FirstChildElement("Name")->GetText();
		_factionListXML->_factionXMLDataVector.back()->_entityListFileName = factionElement->FirstChildElement("EntityList")->GetText();
		_factionListXML->_factionXMLDataVector.back()->_actionListFileName = factionElement->FirstChildElement("ActionList")->GetText();

		//Now parse and create the entity list
		int errorOut;
		EntityListXML* entityListXML = _entityListXMLCreator->CreateFromFile(_factionListXML->_factionXMLDataVector.back()->_entityListFileName, &errorOut);
		_factionListXML->_factionXMLDataVector.back()->_entityListXML = entityListXML;

		//Now parse and create the action list
		ActionListXML* actionListXML = _actionListXMLCreator->CreateFromFile(_factionListXML->_factionXMLDataVector.back()->_actionListFileName, &errorOut);
		_factionListXML->_factionXMLDataVector.back()->_actionListXML = actionListXML;

		ProjectileListXML* projectileListXML = _projectileListXMLCreator->CreateFromEntityList(entityListXML);
		_factionListXML->_factionXMLDataVector.back()->_projectileListXML = projectileListXML;

		DebugOutput("Added faction: %s\n", _factionListXML->_factionXMLDataVector.back()->_name.c_str());

		factionElement = factionElement->NextSiblingElement("Faction");
	}

	return 0;
}

FactionListXML* FactionListXMLParser::GetFactionListXML()
{
	return _factionListXML;
}
