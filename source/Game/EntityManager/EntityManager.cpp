#include "EntityManager.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/LogicModules/LMEntityEventHandler.h"
#include "Game/LogicModules/LMEntityProcessEventHandler.h"

#include "Game/Player/PlayerList.h"
#include "Game/Entity/EntityList.h"
#include "Game/LOSMap/LOSMap.h"

#include "Game/EntityManager/EntityMap.h"
#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"

#include "Game/EntityMovement/EntityCollision.h"

#include "Game/Entity/EntityProcessEventQueue.h"

#include "Game/EntityManager/EntityAnimationHandler.h"
#include "Game/Input/Input.h"

EntityManager::EntityManager()
{
	_entityRenderer = NULL;
	_entitySelectionHandler = NULL;

	_debugEntityRenderer = NULL;
	_debugDrawFlag = false;

	_entityAnimationHandler = NULL;
}

EntityManager::~EntityManager()
{
	delete _entityRenderer;
	delete _entitySelectionHandler;

	delete _debugEntityRenderer;

	delete _entityAnimationHandler;
}

int EntityManager::Init()
{
	_entityRenderer = new EntityRenderer();
	_entityRenderer->Init();
	_entitySelectionHandler = new EntitySelectionHandler();
	_entitySelectionHandler->Init();

	_debugEntityRenderer = new DebugEntityRenderer();
	_debugEntityRenderer->Init();

	_entityAnimationHandler = new EntityAnimationHandler();
	_entityAnimationHandler->Init();

	return 0;
}

int EntityManager::Draw(World* world)
{
	//_entityRenderer->PhageModelCreate();
	_entityRenderer->RenderEntities(world, world->GetEntityList());


	if (world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, SDLK_n))
		_debugDrawFlag = !_debugDrawFlag;
	//Debug rendering
	if (_debugDrawFlag)
		_debugEntityRenderer->RenderEntityStatuses(world->GetEntityList());

	return 0;
}

int EntityManager::PreProcessEntities(World* world)
{
	EntityList* entityList = world->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			LMEntityEventHandler::HandleEntityEventPreProcess(world, entityList->Get(i));

		if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			LMEntityProcessEventHandler::HandleEntityEventPreProcess(world, entityList->Get(i));
	}

	return 0;
}

int EntityManager::ProcessEntities(World* world)
{
	EntityList* entityList = world->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			LMEntityEventHandler::HandleEntityEventProcess(world, entityList->Get(i));

		if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			LMEntityProcessEventHandler::HandleEntityEventProcess(world, entityList->Get(i));
	}

	//world->GetLOSMap()->Clear();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (!entityList->Get(i)->GetEntityBase()->GetLOSPointsPointer())
		{
			LOS::LOSPoints* points = new LOS::LOSPoints();
			//LOS::LOSPoints points;
			world->GetLOS()->CreateLOSPoints(20.0f, points);
			entityList->Get(i)->GetEntityBase()->SetLOSPoints(points);
		}
		world->GetLOS()->SetLineOfSight(entityList->Get(i)->Pos2D(), entityList->Get(i)->GetEntityBase()->GetLOSPointsPointer());
		//world->GetLOSMap()->SetLineOfSight(world->GetCamera(), glm::vec2(entityList->Get(i)->PosX(), entityList->Get(i)->PosZ()), 2.0f);
		_entityAnimationHandler->ProcessAnimation(entityList->Get(i));
	}

	return 0;
}

int EntityManager::PostProcessEntities(World* world)
{
	EntityList* entityList = world->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			LMEntityEventHandler::HandleEntityEventPostProcess(world, entityList->Get(i));

		if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			LMEntityProcessEventHandler::HandleEntityEventPostProcess(world, entityList->Get(i));
	}

	for (int i = 0; i < entityList->Size(); i++)
	{
		if (entityList->Get(i)->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_UNIT)
		{
			world->GetEntityCollision()->StartCollisionCheck(world, entityList->Get(i));
			world->GetEntityCollision()->CalculateEntityCollisions(world, entityList->Get(i));
			//world->GetEntityCollision()->CalculateBuildingCollisions(world, entityList->Get(i));
			world->GetEntityCollision()->CalculateMapCollisions(world, entityList->Get(i));
			world->GetEntityCollision()->HandleCollisions(world, entityList->Get(i));
		}
	}

	return 0;
}

int EntityManager::HandleEntitySelection(World* world)
{
	_entitySelectionHandler->CalcMouseOver(world);

	return 0;
}

int EntityManager::SetOrthoSquare(World* world, glm::vec4* points)
{
	_entitySelectionHandler->SetOrthoSquare(world, points);

	return 0;
}

int EntityManager::SelectMouseOver(World* world)
{
	_entitySelectionHandler->SelectMouseOver(world);

	return 0;
}
