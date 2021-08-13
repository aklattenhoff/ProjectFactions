#include "LMMovement.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Math/EuclideanVector2D.h"

#include "Game/Entity/EntityEventQueue.h"
#include "Game/Entity/EntityStats.h"

#include "Game/EntityManager/EntityMap.h"
#include "Game/EntityMovement/EntityMovement.h"

//#include "Game/Terrain/Terrain.h"

int LMMovement::ProcessMovement(World* world, Entity* entity, int phase)
{
	switch (phase)
	{
	case 0:
		CreatePath(world, entity);
		CalculateInitialVectorToPathNode(world, entity);
		break;
	case 1:
		CalculateCollisionAvoidance(world, entity);
		break;
	case 2:
		MoveAlongVector(world, entity);
		ResolveCollision(world, entity);
		CleanUp(world, entity);
		break;
	}

	return 0;
}

int LMMovement::CreatePath(World* world, Entity* entity)
{
	//Get the event from the entity's EventQueue
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (!evnt->initialized)
	{
		world->GetEntityMovement()->StartMovement(world, entity, evnt);
		evnt->initialized = true;
	}

	if (evnt->requestedPath) return 0;

	//If there is already a path, we don't need to create one and just return
	if (evnt->path.Size() > 0) return 0;

	//Get the start and end point of path to be created
	glm::vec2 startPoint = entity->Pos2D();
	glm::vec2 endPoint = evnt->moveToPosition;
	//If a destination entity was set in the event, then overwrite the end point with that
	if (evnt->entity) endPoint = evnt->entity->Pos2D();
	if (evnt->eventType == EntityEvent::GATHER_FROM_INSTANCE)
	{
		if (evnt->entity->GetEntityBase()->GatherArea()->Size() > 0)
		{
			glm::vec3 midP = evnt->entity->GetTranslationVector() + evnt->entity->GetEntityBase()->GatherArea()->Get(0).midpoint;
			//endPoint = glm::vec2(midP.x, midP.z);
			//evnt->moveToPosition = endPoint;
		}
	}

	//These are soft start and end points that will be moved if they are blocked
	glm::vec2 pointA = startPoint;
	glm::vec2 pointB = endPoint;

	world->GetEntityMovement()->RequestPath(pointA, pointB, world, entity, evnt);

	return 0;
}

int LMMovement::CalculateInitialVectorToPathNode(World* world, Entity* entity)
{
	//Get the event queue for the entity so that we can check if there is a path to follow
	EntityEventQueue* entityEventQueue = entity->GetEventQueue();
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	world->GetEntityMovement()->CalculateVectors(world, entity, evnt);

	return 0;
}

int LMMovement::CalculateCollisionAvoidance(World* world, Entity* entity)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();
	world->GetEntityMovement()->CalculateMovement(world, entity, evnt);

	return 0;
}

int LMMovement::MoveAlongVector(World* world, Entity* entity)
{
	world->GetEntityMovement()->MoveEntity(world, entity);

	return 0;
}

int LMMovement::ResolveCollision(World* world, Entity* entity)
{
	return 0;
}

int LMMovement::CleanUp(World* world, Entity* entity)
{
	world->GetEntityMovement()->CleanUp(world, entity);

	return 0;
}
