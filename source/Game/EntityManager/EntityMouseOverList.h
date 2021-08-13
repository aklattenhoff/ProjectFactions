#ifndef _ENTITY_MOUSE_OVER_LIST_H
#define _ENTITY_MOUSE_OVER_LIST_H

#include <string>
#include <vector>
#include <map>

#include "Game/Entity/Entity.h"

class EntityMouseOverList
{
public:
	EntityMouseOverList();
	~EntityMouseOverList();

	int Init();
	int Add(Entity* entity);
	int Size();
	Entity* At(int index);
	Entity* Back();
	//Entity* At(std::string str);

	int PopBack();
	int Clear();
private:
	std::vector<Entity*> _entityMouseOverVector;
	std::map<std::string, Entity*> _entityMouseOverMap;
};

#endif
