#ifndef _ENTITY_MAP_H
#define _ENTITY_MAP_H

#include <vector>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Entity/Entity.h"

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
private:
	int AddUnit(Entity* entity);
	int AddBuilding(Entity* entity);
public:
	int Remove(Entity* entity);
	int CalcMovement(Entity* entity);
	EntityCell GetEntityCell(glm::ivec2 cell);
	EntityArea GetEntityArea(glm::vec2 center, float radius);

	int AddDestination(Entity* entity, glm::vec2 destination);
	int RemoveDestination(Entity* entity);
	int CalcDestinationMovement(Entity* entity, glm::vec2 destination);
	EntityCell GetEntityDestinationCell(glm::ivec2 cell);
	EntityArea GetEntityDestinationArea(glm::vec2 center, float radius);

	int ClearCheckedByMap();
	Entity* GetCheckedByMemoryAddress(Entity* entity, int value);
	int SetCheckedByMemoryAddress(Entity* entity, Entity* checkedBy, int value);

	//REMOVE: This is probably just for debugging
	int GetNumberOfEntities();
	//END REMOVE
private:

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
	EntityCell* _entityDestinationMap;

	std::map<Entity*, glm::ivec4> _entityPointMap;
	std::map<Entity*, glm::ivec4> _entityDestinationPointMap;

	std::map<Entity*, CheckedBy> _checkedByMap;
};
#endif
