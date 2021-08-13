#include "EntityBaseList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityBase.h"

EntityBaseList::EntityBaseList()
{
}

EntityBaseList::~EntityBaseList()
{
}

int EntityBaseList::Add(EntityBase* entityBase)
{
	_entityBaseVector.push_back(entityBase);
	_entityBaseMap.emplace(entityBase->GetEntityName(), entityBase);

	return 0;
}

EntityBase* EntityBaseList::At(std::string str)
{
	EntityBase* entityBase = NULL;
	try
	{
		entityBase = _entityBaseMap.at(str);
	}
	catch (const std::out_of_range& err)
	{
		return NULL;
	}
	return entityBase;
}
