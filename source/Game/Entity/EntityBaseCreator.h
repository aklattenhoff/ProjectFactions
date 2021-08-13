#ifndef _ENTITY_BASE_CREATOR_H
#define _ENTITY_BASE_CREATOR_H

#include "Game/Entity/EntityBase.h"

#include "Parser/EntityXMLParser.h"

class EntityBaseCreator
{
public:
	EntityBaseCreator();
	~EntityBaseCreator();

	int Init();

	EntityBase* CreateFromFile(const char* fileName, int factionIndex = 0);

private:
	EntityXMLParser* _entityXMLParser;
};

#endif
