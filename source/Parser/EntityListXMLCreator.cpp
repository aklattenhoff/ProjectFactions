#include "EntityListXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/EntityXMLCreator.h"
#include "Parser/EntityListXML.h"

EntityListXMLCreator::EntityListXMLCreator()
{
	_entityXMLCreator = NULL;
}

EntityListXMLCreator::~EntityListXMLCreator()
{
	delete _entityXMLCreator;
}

int EntityListXMLCreator::Init()
{
	_entityXMLCreator = new EntityXMLCreator();
	_entityXMLCreator->Init();

	return 0;
}

EntityListXML* EntityListXMLCreator::CreateFromFile(std::string fileName, int* errorOut)
{
	EntityListXML* entityListXML = NULL;

	tinyxml2::XMLDocument xmlEntityList;
	xmlEntityList.LoadFile(fileName.c_str());

	tinyxml2::XMLElement* entityElement = NULL;
	if (xmlEntityList.FirstChildElement("EntityList"))
		entityElement = xmlEntityList.FirstChildElement("EntityList")->FirstChildElement("Include")->FirstChildElement("Entity");
	else
	{
		DebugOutputFatalError("No EntityList tag found in entity list file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = -1;
		return NULL;
	}

	entityListXML = new EntityListXML();
	entityListXML->Init();

	while (entityElement)
	{
		EntityXML* entityXML = _entityXMLCreator->CreateFromFile(entityElement->GetText());
		entityListXML->_entityXMLVector.push_back(entityXML);

		entityElement = entityElement->NextSiblingElement("Entity");
	}

	//Create the proplist
	tinyxml2::XMLElement* propElement = NULL;
	if (xmlEntityList.FirstChildElement("PropList"))
		propElement = xmlEntityList.FirstChildElement("PropList")->FirstChildElement("Include")->FirstChildElement("Prop");
	else
	{
		DebugOutputFatalError("No PropList tag found in entity list file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = -1;
		return NULL;
	}

	while (propElement)
	{
		entityListXML->_propFileVector.push_back(propElement->GetText());

		printf("Added prop: %s\n", propElement->GetText());

		propElement = propElement->NextSiblingElement("Prop");
	}

	//Create the projectile list
	tinyxml2::XMLElement* projectileElement = NULL;
	if (xmlEntityList.FirstChildElement("ProjectileList"))
		projectileElement = xmlEntityList.FirstChildElement("ProjectileList")->FirstChildElement("Include")->FirstChildElement("Projectile");
	else
	{
		DebugOutputFatalError("No ProjectileList tag found in entity list file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = -1;
		return NULL;
	}

	while (projectileElement)
	{
		entityListXML->_projectileFileVector.push_back(projectileElement->GetText());

		printf("Added projectile: %s\n", projectileElement->GetText());

		projectileElement = projectileElement->NextSiblingElement("Projectile");
	}

	return entityListXML;
}
