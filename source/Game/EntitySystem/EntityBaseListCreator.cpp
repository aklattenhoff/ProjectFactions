#include "EntityBaseListCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityBaseCreator.h"
#include "Parser/Parser.h"
#include "Game/EntitySystem/EntityBaseList.h"

//Parser
#include "Parser/FactionListXML.h"
#include "Parser/FactionXMLData.h"
#include "Parser/EntityListXML.h"
#include "Parser/EntityXML.h"

EntityBaseListCreator::EntityBaseListCreator()
{
	_entityBaseCreator = NULL;
}

EntityBaseListCreator::~EntityBaseListCreator()
{
	delete _entityBaseCreator;
}

int EntityBaseListCreator::Init()
{
	_entityBaseCreator = new EntityBaseCreator();
	_entityBaseCreator->Init();

	return 0;
}

EntityBaseList* EntityBaseListCreator::CreateEntityBaseList(Parser* parser)
{
	EntityBaseList* entityBaseList = new EntityBaseList();
	
	std::vector<FactionXMLData*> factionXMLDataVector = parser->GetFactionListXML()->GetFactionXMLDataVector();
	for (unsigned int i = 0;i < factionXMLDataVector.size();i ++)
	{
		std::vector<EntityXML*> entityXMLVector = factionXMLDataVector[i]->GetEntityListXML()->GetEntityXMLVector();
		for (unsigned int j = 0;j < entityXMLVector.size();i ++)
		{
			if (!entityBaseList->At(entityXMLVector[j]->name))
			{
				EntityBase* entityBase = _entityBaseCreator->CreateEntityBase(entityXMLVector[j]);
				entityBaseList->Add(entityBase);
				DebugOutput("Created EntityBase: %s\n", entityXMLVector[j]->name.c_str());
			}
		}
	}

	return entityBaseList;
}
