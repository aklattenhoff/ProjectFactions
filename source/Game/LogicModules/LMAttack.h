#ifndef _LM_ATTACK_H
#define _LM_ATTACK_H

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMAttack
{
	int Process(World* world, Entity* entity, int phase);
	int Attack(World* world, Entity* entity, int phase);
	int CheckDistance(World* world, Entity* entity, int phase);
}

#endif
