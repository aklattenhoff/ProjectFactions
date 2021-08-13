#include "EntityMouseOverList.h"

#include "Game/EntitySystem/EntityBase.h"

EntityMouseOverList::EntityMouseOverList()
{
}

EntityMouseOverList::~EntityMouseOverList()
{
}

int EntityMouseOverList::Init()
{
	return 0;
}

int EntityMouseOverList::Add(Entity* entity)
{
	_entityMouseOverVector.push_back(entity);
	_entityMouseOverMap.emplace(entity->GetEntityBase()->GetEntityName(), entity);

	return 0;
}

int EntityMouseOverList::Size()
{
	return _entityMouseOverVector.size();
}

Entity* EntityMouseOverList::At(int index)
{
	return _entityMouseOverVector.at(index);
}

Entity* EntityMouseOverList::Back()
{
	return _entityMouseOverVector.back();
}

int EntityMouseOverList::PopBack()
{
	_entityMouseOverVector.pop_back();

	return 0;
}

int EntityMouseOverList::Clear()
{
	_entityMouseOverVector.clear();
	_entityMouseOverMap.clear();

	return 0;
}
