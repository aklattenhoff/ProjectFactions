#include "LMEntityProcessEventHandler.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/EntityProcessEvent.h"
#include "Game/Entity/EntityProcessEventQueue.h"

#include "Game/LogicModules/LMTrain.h"

int RouteEventToLogicModule(World* world, Entity* entity, EntityProcessEvent* e, int phase)
{
	switch (e->eventType)
	{
	case EntityProcessEvent::TRAIN_ENTITY:
		LMTrain::Process(world, entity, phase);
		break;
	}

	return 0;
}

int LMEntityProcessEventHandler::HandleEntityEventPreProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetProcessEventQueue()->GetFrontEvent(), 0);

	return 0;
}

int LMEntityProcessEventHandler::HandleEntityEventProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetProcessEventQueue()->GetFrontEvent(), 1);

	return 0;
}

int LMEntityProcessEventHandler::HandleEntityEventPostProcess(World* world, Entity* entity)
{
	RouteEventToLogicModule(world, entity, entity->GetProcessEventQueue()->GetFrontEvent(), 2);

	return 0;
}
