#ifndef _LM_ENTITY_EVENT_HANDLER
#define _LM_ENTITY_EVENT_HANDLER

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMEntityEventHandler
{
	int HandleEntityEventPreProcess(World* world, Entity* entity);
	int HandleEntityEventProcess(World* world, Entity* entity);
	int HandleEntityEventPostProcess(World* world, Entity* entity);
}

#endif
