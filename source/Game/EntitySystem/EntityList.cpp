#include "EntityList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityList::EntityList()
{
}

EntityList::~EntityList()
{
}

int EntityList::Add(Entity* entity)
{
	entityVector.push_back(entity);
	
	return 0;
}

Entity* EntityList::Get(int index)
{
	return entityVector[index];
}

int EntityList::Size()
{
	return entityVector.size();
}
