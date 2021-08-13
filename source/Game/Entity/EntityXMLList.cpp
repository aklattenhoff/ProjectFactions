#include "EntityXMLList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityXMLList::EntityXMLList()
{
	entityXMLParser = NULL;
}

EntityXMLList::~EntityXMLList()
{
	delete entityXMLParser;
}

int EntityXMLList::Init()
{
	entityXMLParser = new EntityXMLParser();

	return 0;
}

int EntityXMLList::ParseEntityXMLList(const char* fileName, int factionIndex, EntityBaseList* entityBaseList)
{
	//TODO: Add lots of error checking here!

	tinyxml2::XMLDocument xmlEntityListFile;
	xmlEntityListFile.LoadFile(fileName);

	tinyxml2::XMLElement* xmlElement_Entity = xmlEntityListFile.FirstChildElement("ObjectList")->FirstChildElement("Include")->FirstChildElement("Object");

	//Iterate through all entities, adding them to the list, until we reach the last one
	while (xmlElement_Entity)
	{
		entityBaseList->Add(entityXMLParser->Parse(xmlElement_Entity->FirstChild()->ToText()->Value(), factionIndex));

		xmlElement_Entity = xmlElement_Entity->NextSiblingElement("Object");
	}

	return 0;
}
