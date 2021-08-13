#include "EntityMap.h"

#include "Game/Entity/EntityStats.h"

EntityMap::EntityMap()
{

}

EntityMap::~EntityMap()
{

}

int EntityMap::Init(int w, int h, int resolutionPerCell)
{
	_cellX = w;
	_cellY = h;
	_resolution = 1;//resolutionPerCell;

	_entityMap = new EntityCell[_cellX*_cellY];
	_entityDestinationMap = new EntityCell[_cellX*_cellY];

	return 0;
}

int EntityMap::Add(Entity* entity)
{

	//DebugOutput("Added entity to map: %s\n", entity->GetEntityBase()->EntityName().c_str());
	if (entity->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_UNIT)
		AddUnit(entity);
	else if (entity->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_BUILDING)
		AddBuilding(entity);

	return 0;
}

int EntityMap::AddUnit(Entity* entity)
{
	glm::ivec4 map;

	map[0] = (int)(entity->PosX() - entity->GetEntityStats()->Radius()) / _resolution; map[1] = (int)(entity->PosX() + entity->GetEntityStats()->Radius()) / _resolution;
	map[2] = (int)(entity->PosZ() - entity->GetEntityStats()->Radius()) / _resolution; map[3] = (int)(entity->PosZ() + entity->GetEntityStats()->Radius()) / _resolution;

	//Clamp to map size
	map[0] = glm::clamp(map[0], 0, _cellX - 1);
	map[1] = glm::clamp(map[1], 0, _cellX - 1);
	map[2] = glm::clamp(map[2], 0, _cellY - 1);
	map[3] = glm::clamp(map[3], 0, _cellY - 1);

	for (int j = map[2]; j <= map[3]; j++)
		for (int i = map[0]; i <= map[1]; i++)
			_entityMap[i + j*_cellX].push_back(entity);

	//Save the points for the entity
	_entityPointMap.emplace(entity, map);

	return 0;
}

int EntityMap::AddBuilding(Entity* entity)
{
	glm::vec3 tempVector;
	float tempMagnitude;
	glm::vec3 longestVector;
	float longestMagnitude = 0.0f;

	//Since the bounding boxes already contain vectors from the midpoint to the points, we just need to find which is the longest vector from the center
	for (int i = 0; i < (int)entity->GetEntityBase()->BoundingBox2D()->Size(); i++)
	{
		BoundingBox2D bb2D = entity->GetEntityBase()->BoundingBox2D()->Get(i);

		for (int vecInc = 1; vecInc <= 4; vecInc++)
		{
			if (vecInc == 1) tempVector = bb2D.midpoint + bb2D.p1Vec;
			else if (vecInc == 2) tempVector = bb2D.midpoint + bb2D.p2Vec;
			else if (vecInc == 3) tempVector = bb2D.midpoint + bb2D.p3Vec;
			else tempVector = bb2D.midpoint + bb2D.p4Vec;
			 
			tempMagnitude = glm::length(tempVector);
			if (tempMagnitude > longestMagnitude)
			{
				longestVector = tempVector;
				longestMagnitude = tempMagnitude;
			}
		}
	}

	DebugOutput("LongestMag: %f\n", longestMagnitude);

	glm::ivec4 map;

	map[0] = (int)(entity->PosX() - longestMagnitude) / _resolution; map[1] = (int)(entity->PosX() + longestMagnitude) / _resolution;
	map[2] = (int)(entity->PosZ() - longestMagnitude) / _resolution; map[3] = (int)(entity->PosZ() + longestMagnitude) / _resolution;

	//Clamp to map size
	map[0] = glm::clamp(map[0], 0, _cellX - 1);
	map[1] = glm::clamp(map[1], 0, _cellX - 1);
	map[2] = glm::clamp(map[2], 0, _cellY - 1);
	map[3] = glm::clamp(map[3], 0, _cellY - 1);

	for (int j = map[2]; j <= map[3]; j++)
		for (int i = map[0]; i <= map[1]; i++)
			_entityMap[i + j*_cellX].push_back(entity);

	//Save the points for the entity
	_entityPointMap.emplace(entity, map);

	return 0;
}

int EntityMap::Remove(Entity* entity)
{
	glm::ivec4 map = _entityPointMap.at(entity);

	for (int j = map[2]; j <= map[3]; j++)
		for (int i = map[0]; i <= map[1]; i++)
			_entityMap[i + j*_cellX].remove(entity);

	//Erase the points from the point map for the entity
	_entityPointMap.erase(entity);

	return 0;
}

int EntityMap::CalcMovement(Entity* entity)
{
	glm::ivec4 map_prev = _entityPointMap.at(entity);
	glm::ivec4 map_curr;

	map_curr[0] = (int)(entity->PosX() - entity->GetEntityStats()->Radius()) / _resolution; map_curr[1] = (int)(entity->PosX() + entity->GetEntityStats()->Radius()) / _resolution;
	map_curr[2] = (int)(entity->PosZ() - entity->GetEntityStats()->Radius()) / _resolution; map_curr[3] = (int)(entity->PosZ() + entity->GetEntityStats()->Radius()) / _resolution;

	//Clamp to map size
	map_curr[0] = glm::clamp(map_curr[0], 0, _cellX - 1);
	map_curr[1] = glm::clamp(map_curr[1], 0, _cellX - 1);
	map_curr[2] = glm::clamp(map_curr[2], 0, _cellY - 1);
	map_curr[3] = glm::clamp(map_curr[3], 0, _cellY - 1);
	//if (map_curr[0] < 0) map_curr[0] = 0; if (map_curr[0] > _cellX-1) map_curr[0] = _cellX-1;
	//if (map_curr[1] < 0) map_curr[1] = 0; if (map_curr[1] > _cellX-1) map_curr[1] = _cellX-1;
	//if (map_curr[2] < 0) map_curr[2] = 0; if (map_curr[2] > _cellY-1) map_curr[2] = _cellY-1;
	//if (map_curr[3] < 0) map_curr[3] = 0; if (map_curr[3] > _cellY-1) map_curr[3] = _cellY-1;

	if ((map_prev[0] != map_curr[0] || map_prev[1] != map_curr[1]) ||
		(map_prev[2] != map_curr[2] || map_prev[3] != map_curr[3]))
	{
		Remove(entity);
		Add(entity);
	}

	return 0;
}

EntityCell EntityMap::GetEntityCell(glm::ivec2 cell)
{
	if (cell.x < 0) cell.x = 0; if (cell.x > _cellX-1) cell.x = _cellX-1;
	if (cell.y < 0) cell.y = 0; if (cell.y > _cellX-1) cell.y = _cellX-1;

	return _entityMap[cell.x+cell.y*_cellX];
}

EntityArea EntityMap::GetEntityArea(glm::vec2 center, float radius)
{
	EntityArea returnArea;

	int topX = (int)(center.x-radius) / _resolution;
	int topY = (int)(center.y-radius) / _resolution;
	int botX = (int)(center.x+radius) / _resolution;
	int botY = (int)(center.y+radius) / _resolution;

	//Clamp to map size
	if (topX < 0) topX = 0; if (topX > _cellX-1) topX = _cellX-1;
	if (botX < 0) botX = 0; if (botX > _cellX-1) botX = _cellX-1;
	if (topY < 0) topY = 0; if (topY > _cellY-1) topY = _cellY-1;
	if (botY < 0) botY = 0; if (botY > _cellY-1) botY = _cellY-1;

	for (int j = topY;j <= botY;j ++)
		for (int i = topX;i <= botX;i ++)
			returnArea.push_back(&_entityMap[i+j*_cellX]);

	return returnArea;
}

int EntityMap::AddDestination(Entity* entity, glm::vec2 destination)
{
	glm::ivec4 map;

	map[0] = (int)(destination.x - entity->GetEntityStats()->Radius()) / _resolution; map[1] = (int)(destination.x + entity->GetEntityStats()->Radius()) / _resolution;
	map[2] = (int)(destination.y - entity->GetEntityStats()->Radius()) / _resolution; map[3] = (int)(destination.y + entity->GetEntityStats()->Radius()) / _resolution;

	//Clamp to map size
	map[0] = glm::clamp(map[0], 0, _cellX - 1);
	map[1] = glm::clamp(map[1], 0, _cellX - 1);
	map[2] = glm::clamp(map[2], 0, _cellY - 1);
	map[3] = glm::clamp(map[3], 0, _cellY - 1);

	for (int j = map[2]; j <= map[3]; j++)
		for (int i = map[0]; i <= map[1]; i++)
			_entityDestinationMap[i + j*_cellX].push_back(entity);

	//Save the points for the entity
	_entityDestinationPointMap.emplace(entity, map);

	return 0;
}

int EntityMap::RemoveDestination(Entity* entity)
{
	glm::ivec4 map;
	try
	{
		map = _entityDestinationPointMap.at(entity);
	}
	catch (const std::out_of_range)
	{
		return 0;
	}

	for (int j = map[2]; j <= map[3]; j++)
		for (int i = map[0]; i <= map[1]; i++)
			_entityDestinationMap[i + j*_cellX].remove(entity);

	//Erase the points from the point map for the entity
	_entityDestinationPointMap.erase(entity);

	return 0;
}

int EntityMap::CalcDestinationMovement(Entity* entity, glm::vec2 destination)
{
	glm::ivec4 map_prev = _entityPointMap.at(entity);
	glm::ivec4 map_curr;

	map_curr[0] = (int)(destination.x - entity->GetEntityStats()->Radius()) / _resolution; map_curr[1] = (int)(destination.x + entity->GetEntityStats()->Radius()) / _resolution;
	map_curr[2] = (int)(destination.y - entity->GetEntityStats()->Radius()) / _resolution; map_curr[3] = (int)(destination.y + entity->GetEntityStats()->Radius()) / _resolution;

	//Clamp to map size
	map_curr[0] = glm::clamp(map_curr[0], 0, _cellX - 1);
	map_curr[1] = glm::clamp(map_curr[1], 0, _cellX - 1);
	map_curr[2] = glm::clamp(map_curr[2], 0, _cellY - 1);
	map_curr[3] = glm::clamp(map_curr[3], 0, _cellY - 1);

	if ((map_prev[0] != map_curr[0] || map_prev[1] != map_curr[1]) ||
		(map_prev[2] != map_curr[2] || map_prev[3] != map_curr[3]))
	{
		RemoveDestination(entity);
		AddDestination(entity, destination);
	}

	return 0;
}

EntityCell EntityMap::GetEntityDestinationCell(glm::ivec2 cell)
{
	if (cell.x < 0) cell.x = 0; if (cell.x > _cellX - 1) cell.x = _cellX - 1;
	if (cell.y < 0) cell.y = 0; if (cell.y > _cellX - 1) cell.y = _cellX - 1;

	return _entityDestinationMap[cell.x + cell.y*_cellX];
}

EntityArea EntityMap::GetEntityDestinationArea(glm::vec2 center, float radius)
{
	EntityArea returnArea;

	int topX = (int)(center.x - radius) / _resolution;
	int topY = (int)(center.y - radius) / _resolution;
	int botX = (int)(center.x + radius) / _resolution;
	int botY = (int)(center.y + radius) / _resolution;

	//Clamp to map size
	if (topX < 0) topX = 0; if (topX > _cellX - 1) topX = _cellX - 1;
	if (botX < 0) botX = 0; if (botX > _cellX - 1) botX = _cellX - 1;
	if (topY < 0) topY = 0; if (topY > _cellY - 1) topY = _cellY - 1;
	if (botY < 0) botY = 0; if (botY > _cellY - 1) botY = _cellY - 1;

	for (int j = topY; j <= botY; j++)
		for (int i = topX; i <= botX; i++)
			returnArea.push_back(&_entityDestinationMap[i + j*_cellX]);

	return returnArea;
}

int EntityMap::ClearCheckedByMap()
{
	_checkedByMap.clear();

	return 0;
}

Entity* EntityMap::GetCheckedByMemoryAddress(Entity* entity, int value)
{
	CheckedBy returnCheckedBy;

	try
	{
		returnCheckedBy = _checkedByMap.at(entity);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}

	if (returnCheckedBy.frame == value)
		return returnCheckedBy.entity;
	else
		return NULL;
}

int EntityMap::SetCheckedByMemoryAddress(Entity* entity, Entity* checkedBy, int value)
{
	CheckedBy returnCheckedBy;
	returnCheckedBy.entity = checkedBy;
	returnCheckedBy.frame = value;
	_checkedByMap.emplace(entity, returnCheckedBy);

	return 0;
}

int EntityMap::GetNumberOfEntities()
{
	int returnNum = 0;

	for (int i = 0;i < _cellX*_cellY;i ++)
	{
		for (EntityIterator entityIter = _entityMap[i].begin(); entityIter != _entityMap[i].end(); entityIter++)
		{
			returnNum ++;
		}
	}

	return returnNum;
}
