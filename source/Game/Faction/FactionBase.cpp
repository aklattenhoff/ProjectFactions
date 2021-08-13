#include "FactionBase.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

FactionBase::FactionBase()
{
	_entityBaseList = NULL;
}

FactionBase::~FactionBase()
{
	delete _entityBaseList;
}

int FactionBase::Init()
{
	_entityBaseList = new EntityBaseList();

	return 0;
}

int FactionBase::CreateEntityBaseList(const char* fileName, EntityBaseCreator* entityBaseCreator)
{
	//TODO: Add lots of error checking here!

	//Load the entity list file
	tinyxml2::XMLDocument xmlEntityListFile;
	xmlEntityListFile.LoadFile(fileName);

	//Set to the first entity in the file
	tinyxml2::XMLElement* xmlElement_Entity = xmlEntityListFile.FirstChildElement("EntityList")->FirstChildElement("Include")->FirstChildElement("Entity");

	//Iterate through all entities, creating and adding them to the list, until we reach the last one
	while (xmlElement_Entity)
	{
		_entityBaseList->Add(entityBaseCreator->CreateFromFile(xmlElement_Entity->FirstChild()->ToText()->Value()));

		xmlElement_Entity = xmlElement_Entity->NextSiblingElement("Entity");
	}
	
	return 0;
}

EntityBaseList* FactionBase::GetEntityBaseList()
{
	return _entityBaseList;
}
