#ifndef _ENTITY_MAP_H
#define _ENTITY_MAP_H

#include <vector>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/EntitySystem/Entity.h"

typedef std::list<Entity*> EntityCell;
typedef std::vector<EntityCell*> EntityArea;
typedef std::list<Entity*>::const_iterator EntityIterator;

class EntityMap
{
public:
	EntityMap();
	~EntityMap();

	int Init(int w, int h, int resolutionPerCell);
	int Add(Entity* entity);
	int CalcMovement(Entity* entity);

	EntityCell GetEntityCell(glm::ivec2 cell);
	EntityArea GetEntityArea(glm::vec2 center, float radius);

	int ClearCheckedByMap();
	Entity* GetCheckedByMemoryAddress(Entity* entity, int value);
	int SetCheckedByMemoryAddress(Entity* entity, Entity* checkedBy, int value);

	//REMOVE: This is probably just for debugging
	int GetNumberOfEntities();
	//END REMOVE
private:
	int Remove(Entity* entity);

	struct CheckedBy
	{
		Entity* entity;
		int frame;
	};
private:
	int _cellX;
	int _cellY;
	int _resolution;

	EntityCell* _entityMap;

	std::map<Entity*, glm::ivec4> _entityPointMap;
	std::map<Entity*, CheckedBy> _checkedByMap;
};
#endif
