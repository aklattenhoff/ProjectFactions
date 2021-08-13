#ifndef _LM_GATHER_RESOURCE_H
#define _LM_GATHER_RESOURCE_H

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMGatherResource
{
	int Process(World* world, Entity* entity, int phase);
	int GatherResource(World* world, Entity* entity, int phase);
	int CheckDistance(World* world, Entity* entity, int phase);
}

#endif
