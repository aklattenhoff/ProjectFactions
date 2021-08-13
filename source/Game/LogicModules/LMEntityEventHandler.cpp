#include "LMEntityEventHandler.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/EntityEvent.h"

#include "Game/LogicModules/LMMovement.h"
#include "Game/LogicModules/LMGatherResource.h"
#include "Game/LogicModules/LMBuild.h"
#include "Game/LogicModules/LMAttack.h"

int RouteEventToLogicModule(World* world, Entity* entity, EntityEvent* e, int phase)
{
	switch (e->eventType)
	{
	case EntityEvent::MOVE_TO_POSITION:
		entity->SetEntityStatus(Entity::STATUS_WALKING);
		LMMovement::ProcessMovement(world, entity, phase);
		break;
	case EntityEvent::MOVE_IN_SQUADRON_TO_POSITION:
		entity->SetEntityStatus(Entity::STATUS_WALKING);
		LMMovement::ProcessMovement(world, entity, phase);
		break;
	case EntityEvent::MOVE_TO_INSTANCE:
		entity->SetEntityStatus(Entity::STATUS_WALKING);
		LMMovement::ProcessMovement(world, entity, phase);
		break;
	case EntityEvent::GATHER_FROM_INSTANCE:
		entity->SetEntityStatus(Entity::STATUS_GATHERING);
		LMGatherResource::Process(world, entity, phase);
		break;
	case EntityEvent::BUILD_INSTANCE:
		entity->SetEntityStatus(Entity::STATUS_BUILDING);
		LMBuild::Process(world, entity, phase);
		break;
	case EntityEvent::ATTACK_INSTANCE:
		entity->SetEntityStatus(Entity::STATUS_ATTACKING);
		LMAttack::Process(world, entity, phase);
		break;
	}

	return 0;
}

int LMEntityEventHandler::HandleEntityEventPreProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetEventQueue()->GetFrontEvent(), 0);

	return 0;
}

int LMEntityEventHandler::HandleEntityEventProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetEventQueue()->GetFrontEvent(), 1);

	return 0;
}

int LMEntityEventHandler::HandleEntityEventPostProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetEventQueue()->GetFrontEvent(), 2);

	return 0;
}
