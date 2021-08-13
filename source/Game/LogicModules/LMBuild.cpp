#include "LMBuild.h"

#include "Game/LogicModules/LMMovement.h"

#include "Game/Entity/EntityEventQueue.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityBase.h"

//#include "Game/Terrain/Terrain.h"
//#include "Game/Terrain/TerrainMap.h"

int LMBuild::Process(World* world, Entity* entity, int phase)
{
	switch (phase)
	{
	case 0:
		CheckDistance(world, entity, phase);
		break;
	case 1:
		CheckDistance(world, entity, phase);
		Build(world, entity, phase);
		break;
	case 2:
		CheckDistance(world, entity, phase);
		break;
	}

	return 0;
}

int LMBuild::Build(World* world, Entity* entity, int phase)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (evnt->needToMove)
		return 0;

	//Build entity
	if (!evnt->entity->IsBuilt())
		evnt->entity->AddCurrentBuildPoints(1.0f/60.0f);
	else
	{
		entity->GetEventQueue()->DequeueAndDelete();
		entity->SetEntityStatus(Entity::STATUS_IDLING);
	}

	return 0;
}

int LMBuild::CheckDistance(World* world, Entity* entity, int phase)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (phase == 0)
	{
		//Calculate the distance
		float distance = sqrt(pow(evnt->entity->PosX() - entity->PosX(), 2.0f)+pow(evnt->entity->PosZ() - entity->PosZ(), 2.0f));

		//Set whether movement is needed for this event
		if (distance > 2.0f/*ADDVAR: Min distance to build*/)
			evnt->needToMove = true;
		else
			evnt->needToMove = false;

		//If we need to move and there is no path, or the target entity has moved off target, then create a new path.
		if (evnt->needToMove && (evnt->path.Size() <= 0 ||
			sqrt(pow(evnt->entity->PosX() - evnt->path.Back().x, 2.0f)+pow(evnt->entity->PosZ() - evnt->path.Back().y, 2.0f)) > 4.0f /*ADDVAR: Min distance to build times 2*/))
		{
			evnt->path.Clear();

			//If the path is a straight shot, then we can create it quickly.
			//Otherwise, just let the movement module handle creating it.
			//float terrRes = (float)world->GetTerrain()->getTerrainMap()->GetResolution();
			/*if (world->GetTerrain()->getTerrainMap()->CheckClearLine(entity->Pos2D()*terrRes, evnt->entity->Pos2D()*terrRes) == 0)
			{
				evnt->path.PushBack(Path::Node(evnt->entity->Pos2D()));
			}
			else
			{
				int pathResult = world->GetTerrain()->getTerrainMap()->FindPathToInstance(	entity->Pos2D(),
																							evnt->entity->Pos2D(),
																							evnt->entity->GetTerrainMapCellsVectorPointer(),
																							&evnt->path);
				if (evnt->path.Size() > 1)
					evnt->path.EraseFront();
				evnt->requestedPath = true;
				evnt->hasPath = true;
				DebugOutput("Finding Build Path %s to %s\n", entity->GetEntityBase()->EntityName().c_str(), evnt->entity->GetEntityBase()->EntityName().c_str());
				if (pathResult <= 0)
				{
					DebugOutput("Could not find a valid path...Setting default path and trying again\n");
					evnt->path.PushBack(Path::Node(evnt->entity->Pos2D()));
				}
			}*/
			//evnt->requestedPath = true;
			//evnt->hasPath = true;
		}
	}

	//If there is movement is needed, then process movement at each phase
	if (evnt->needToMove)
	{
		entity->SetEntityStatus(Entity::STATUS_WALKING);
		LMMovement::ProcessMovement(world, entity, phase);
	}

	return 0;
}
