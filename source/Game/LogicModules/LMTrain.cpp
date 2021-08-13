#include "LMTrain.h"

#include "Game/LogicModules/LMMovement.h"

#include "Game/ActionManager/ActionProgressQueue.h"

#include "Game/Entity/EntityProcessEventQueue.h"
#include "Game/Entity/EntityProcessEvent.h"

//#include "Game/Terrain/Terrain.h"
//#include "Game/Terrain/TerrainMap.h"

#include "Game/Player/PlayerList.h"
#include "Game/Player/Player.h"
#include "Game/Entity/EntityList.h"
#include "Game/EntityManager/EntityMap.h"

#include "Game/Entity/EntityBase.h"
#include "Renderer/Renderer.h"

int LMTrain::Process(World* world, Entity* entity, int phase)
{
	switch (phase)
	{
	case 0:
		break;
	case 1:
		Train(world, entity, phase);
		break;
	case 2:
		break;
	}

	return 0;
}

int LMTrain::Train(World* world, Entity* entity, int phase)
{
	EntityProcessEvent* evnt = entity->GetProcessEventQueue()->GetFrontEvent();

	if (!evnt->initialized || evnt->timer == 0)
	{
		evnt->initialized = true;
		evnt->timer = SDL_GetTicks();
		evnt->trainPointsRemaining = evnt->trainPoints;
		evnt->debugTrainPoints = evnt->trainPoints;

		DebugOutput("Started training entity %s with train points: %f\n", evnt->entityBase->EntityName().c_str(), evnt->trainPoints);
	}
	else
	{
		float seconds = (float)(SDL_GetTicks() - evnt->timer);
		seconds /= 1000.0f;

		evnt->timer = SDL_GetTicks();

		evnt->trainPointsRemaining -= seconds * (float)RendererSingleton->GetGameSpeed();

		if (evnt->debugTrainPoints - evnt->trainPointsRemaining > 1.0f)
		{
			DebugOutput("Updated training entity %s with train points remaining: %f\n", evnt->entityBase->EntityName().c_str(), evnt->trainPointsRemaining);
			evnt->debugTrainPoints = evnt->trainPointsRemaining;
		}
	}

	if (evnt->trainPointsRemaining <= 0.0f)
	{
		float pX, pY;
		FindPlacementPoint(world, entity, phase, &pX, &pY);

		Entity* placeEntity = evnt->entityBase->CreateNewInstance(pX, pY, 0.0f, 0, world->GetPhageTerrain()->CalculateHeight(pX, pY));
		world->GetPlayerList()->GetAt(0)->GetEntityList()->Add(placeEntity);
		world->GetEntityList()->Add(placeEntity);
		world->GetEntityMap()->Add(placeEntity);

		DebugOutput("Created training entity %s\n", evnt->entityBase->EntityName().c_str());

		//ActionProgressQueue
		world->GetActionProgressQueue()->RemoveActionProgress(evnt);
		//End-ActionProgressQueue
		
		entity->GetProcessEventQueue()->DequeueAndDelete();
	}

	return 0;
}

int LMTrain::FindPlacementPoint(World* world, Entity* entity, int phase, float* x, float* y)
{
	//FIXME: This needs more logic than this, but for now we are just placing it below the entity
	(*x) = entity->GetTranslationVector().x;
	(*y) = entity->GetTranslationVector().z + 7.5f;

	return 0;
}
