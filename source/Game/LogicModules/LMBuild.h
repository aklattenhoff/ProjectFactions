#ifndef _LM_BUILD_H
#define _LM_BUILD_H

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMBuild
{
	int Process(World* world, Entity* entity, int phase);
	int Build(World* world, Entity* entity, int phase);
	int CheckDistance(World* world, Entity* entity, int phase);
}

#endif
