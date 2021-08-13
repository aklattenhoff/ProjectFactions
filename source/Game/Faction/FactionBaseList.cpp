#include "FactionBaseList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

FactionBaseList::FactionBaseList()
{
	_entityBaseCreator = NULL;
}

FactionBaseList::~FactionBaseList()
{
	delete _entityBaseCreator;
}

int FactionBaseList::Init()
{
	_entityBaseCreator = new EntityBaseCreator();
	_entityBaseCreator->Init();

	return 0;
}

int FactionBaseList::CreateAllFactions(const char* fileName)
{
	//Load faction list
	tinyxml2::XMLDocument xmlFactionList;
	xmlFactionList.LoadFile(fileName);

	//Set to the first faction in the file
	tinyxml2::XMLElement* factionElement = NULL;
	if (xmlFactionList.FirstChildElement("FactionList")->FirstChildElement("Faction"))
		factionElement = xmlFactionList.FirstChildElement("FactionList")->FirstChildElement("Faction");
	else
		DebugOutputFatalError("No faction tag found in faction list file: %s\n", fileName);

	//Loop through each faction in the faction list and create the components for the factions
	while (factionElement)
	{
		_factionBaseVector.push_back(new FactionBase());
		_factionBaseVector.back()->Init();
		_factionBaseVector.back()->CreateEntityBaseList(factionElement->FirstChildElement("EntityList")->GetText(), _entityBaseCreator);
		_factionBaseMap.emplace(factionElement->FirstChildElement("Name")->GetText(), _factionBaseVector.back());

		DebugOutput("Added faction: %s\n", factionElement->FirstChildElement("Name")->GetText());

		factionElement = factionElement->NextSiblingElement("Faction");
	}

	return 0;
}

FactionBase* FactionBaseList::At(std::string str)
{
	return _factionBaseMap.at(str);
}
