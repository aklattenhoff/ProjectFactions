#ifndef _ENTITY_XML_LIST_H
#define _ENTITY_XML_LIST_H

#include "Game/Entity/EntityBaseList.h"
#include "Game/Entity/EntityXMLParser.h"

class EntityXMLList
{
public:
	EntityXMLList();
	~EntityXMLList();

	int Init();
	int ParseEntityXMLList(const char* fileName, int factionIndex, EntityBaseList* entityBaseList);
private:
	EntityXMLParser* entityXMLParser;
};

#endif
