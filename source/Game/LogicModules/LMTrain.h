#ifndef _LM_TRAIN_H
#define _LM_TRAIN_H

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMTrain
{
	int Process(World* world, Entity* entity, int phase);
	int Train(World* world, Entity* entity, int phase);
	int FindPlacementPoint(World* world, Entity* entity, int phase, float* x, float* y);
}

#endif
