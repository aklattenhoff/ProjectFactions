#ifndef _LM_MOVEMENT_H
#define _LM_MOVEMENT_H

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMMovement
{
	int ProcessMovement(World* world, Entity* entity, int phase);
	//////////////////////////////////////////////////////////////////////
	//CreatePath(world, entity)
	//Creates a path from the entity to the destination if one has not
	//already been created
	//////////////////////////////////////////////////////////////////////
	int CreatePath(World* world, Entity* entity);
	int CalculateInitialVectorToPathNode(World* world, Entity* entity);
	int CalculateCollisionAvoidance(World* world, Entity* entity);
	int MoveAlongVector(World* world, Entity* entity);
	int ResolveCollision(World* world, Entity* entity);
	int CleanUp(World* world, Entity* entity);
}

#endif
