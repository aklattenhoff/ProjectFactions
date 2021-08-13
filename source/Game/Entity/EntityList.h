#ifndef _ENTITY_LIST_H
#define _ENTITY_LIST_H

#include <vector>

#include "Game/Entity/Entity.h"

class EntityList
{
public:
	EntityList();
	~EntityList();

	int Add(Entity* entity);
	Entity* Get(int index);
	int Remove(Entity* entity);
	int Size();
private:
	std::vector<Entity*> entityVector;
};

#endif
