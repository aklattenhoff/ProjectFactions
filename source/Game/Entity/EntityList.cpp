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

int EntityList::Remove(Entity* entity)
{
	for (unsigned int i = 0;i < entityVector.size();i ++)
	{
		if (entityVector[i] == entity)
		{
			entityVector.erase(entityVector.begin()+i);
			i --;
		}
	}

	return 0;
}

int EntityList::Size()
{
	return entityVector.size();
}
