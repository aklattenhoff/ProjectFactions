#include "EntityStats.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityStats::EntityStats()
{
}

EntityStats::~EntityStats()
{
}

int EntityStats::SetEntityBase(EntityBase* entityBase)
{
	_entityBase = entityBase;

	return 0;
}

float EntityStats::MovementSpeed()
{
	return _entityBase->MovementSpeed();// + modifier
}

float EntityStats::Radius()
{
	return 0.5f;
}

float EntityStats::AwarenessRadius()
{
	return 0.5f;
}

float EntityStats::BuildPoints()
{
	return _entityBase->GetEntityXML()->buildpoints;
}
