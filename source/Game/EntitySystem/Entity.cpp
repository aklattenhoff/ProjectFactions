#include "Entity.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityProcessEventQueue.h"

#include "Game/EntitySystem/EntityBase.h"
#include "Game/EntitySystem/EntityStats.h"

Entity::Entity()
{
	_entityStats = NULL;
	_entityEventQueue = NULL;
	_entityProcessEventQueue = NULL;
	_animationSeconds = ((float)(SDL_GetTicks()) / 1000.0f);
}

Entity::~Entity()
{
	delete _entityStats;
	delete _entityEventQueue;
	delete _entityProcessEventQueue;
}

int Entity::Init()
{
	_entityStats = new EntityStats();
	_entityStats->SetEntityBase(_entityBase);

	_entityEventQueue = new EntityEventQueue();

	_entityProcessEventQueue = new EntityProcessEventQueue();

	_entityStatus = STATUS_IDLING;

	_isMouseOver = false;
	_isSelected = false;

	return 0;
}

EntityBase* Entity::GetEntityBase()
{
	return _entityBase;
}

EntityStats* Entity::GetEntityStats()
{
	return _entityStats;
}

int Entity::ResetToNodeVector()
{
	_toNodeVec.direction = 0.0f;
	_toNodeVec.magnitude = 0.0f;

	return 0;
}

int Entity::ResetMovementVector()
{
	_movementVec.direction = 0.0f;
	_movementVec.magnitude = 0.0f;

	return 0;
}
