#include "EntityEvent.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/SquadronManager/Squadron.h"
#include "Game/SquadronManager/Group.h"
#include "Game/Entity/Entity.h"

EntityEvent::EntityEvent()
{
	moveToPosition = glm::vec2(-1.0f, -1.0f);
	initialized = false;
	needToMove = false;
	requestedPath = false;
	hasPath = false;
	foundGatherType = false;
	hasGatherArea = false;
	atGatherArea = false;
	hasSetGatherRallyPoint = false;
	gatherTimer = -1;
	gatherRate = -1.0f;
	frameCount = -1;
	owner = NULL;
	entity = NULL;
	squadron = NULL;
	group = NULL;
}

EntityEvent::~EntityEvent()
{
	//if (group)
		//group->RemoveEntity(owner);
	//group = NULL;

	if (squadron)
		squadron->RemoveEntity(owner);
	squadron = NULL;

	owner->SetStartedAttack(false);
	owner->ClearAllProjectileCreated();
}
