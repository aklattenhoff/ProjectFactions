#include "EntityEvent.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/SquadronManager/Squadron.h"
#include "Game/SquadronManager/Group.h"

EntityEvent::EntityEvent()
{
	initialized = false;
	needToMove = false;
	owner = NULL;
	entity = NULL;
	squadron = NULL;
	group = NULL;
}

EntityEvent::~EntityEvent()
{
	if (group)
		group->RemoveEntity(owner);
	group = NULL;

	if (squadron)
		squadron->RemoveEntity(owner);
	squadron = NULL;
}
