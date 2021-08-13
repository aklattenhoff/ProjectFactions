#ifndef _ENTITY_SELECTION_LIST_H
#define _ENTITY_SELECTION_LIST_H

#include <string>
#include <vector>
#include <map>

#include "Game/Entity/Entity.h"

class EntitySelectionList
{
public:
	EntitySelectionList();
	~EntitySelectionList();

	int Init();
	int Add(Entity* entity);
	int Size();
	Entity* At(int index);
	//Entity* At(std::string str);

	int Clear();
private:
	std::vector<Entity*> _entitySelectionVector;
	std::map<std::string, Entity*> _entitySelectionMap;
};

#endif
