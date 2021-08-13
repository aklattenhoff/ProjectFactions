#include "EntitySelectionList.h"

#include "Game/Entity/EntityBase.h"

EntitySelectionList::EntitySelectionList()
{
}

EntitySelectionList::~EntitySelectionList()
{
}

int EntitySelectionList::Init()
{
	return 0;
}

int EntitySelectionList::Add(Entity* entity)
{
	_entitySelectionVector.push_back(entity);
	_entitySelectionMap.emplace(entity->GetEntityBase()->EntityName(), entity);

	return 0;
}

int EntitySelectionList::Size()
{
	return _entitySelectionVector.size();
}

Entity* EntitySelectionList::At(int index)
{
	return _entitySelectionVector.at(index);
}

int EntitySelectionList::Clear()
{
	_entitySelectionVector.clear();
	_entitySelectionMap.clear();

	return 0;
}
