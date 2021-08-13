#ifndef _LM_ENTITY_PROCESS_EVENT_HANDLER
#define _LM_ENTITY_PROCESS_EVENT_HANDLER

#include "Game/World.h"

#include "Game/Entity/Entity.h"

namespace LMEntityProcessEventHandler
{
	int HandleEntityEventPreProcess(World* world, Entity* entity);
	int HandleEntityEventProcess(World* world, Entity* entity);
	int HandleEntityEventPostProcess(World* world, Entity* entity);
}

#endif
