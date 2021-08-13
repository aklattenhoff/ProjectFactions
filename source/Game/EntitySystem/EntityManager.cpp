#include "EntityManager.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntitySystem.h"

#include "Game/EntitySystem/EntityList.h"


EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

int EntityManager::Init()
{

	return 0;
}

int EntityManager::Draw(EntitySystem* entitySystem)
{
	//entitySystem->GetEntityRenderer()->RenderEntities(world, world->GetCamera(), world->GetEntityList());

	//Debug rendering
	//entitySystem->GetDebugEntityRenderer()->RenderEntityStatuses(world->GetCamera(), world->GetEntityList());

	return 0;
}

int EntityManager::PreProcessEntities(EntitySystem* entitySystem)
{
	EntityList* entityList = entitySystem->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		//if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			//LMEntityEventHandler::HandleEntityEventPreProcess(world, entityList->Get(i));

		//if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			//LMEntityProcessEventHandler::HandleEntityEventPreProcess(world, entityList->Get(i));
	}

	return 0;
}

int EntityManager::ProcessEntities(EntitySystem* entitySystem)
{
	/*EntityList* entityList = world->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			LMEntityEventHandler::HandleEntityEventProcess(world, entityList->Get(i));

		if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			LMEntityProcessEventHandler::HandleEntityEventProcess(world, entityList->Get(i));
	}

	world->GetLOSMap()->Clear();
	for (int i = 0;i < entityList->Size(); i ++)
		world->GetLOSMap()->SetLineOfSight(world->GetCamera(), glm::vec2(entityList->Get(i)->PosX(), entityList->Get(i)->PosZ()), 2.0f);*/

	return 0;
}

int EntityManager::PostProcessEntities(EntitySystem* entitySystem)
{
	/*EntityList* entityList = world->GetEntityList();
	for (int i = 0;i < entityList->Size(); i ++)
	{
		if (entityList->Get(i)->GetEventQueue()->Size() > 0)
			LMEntityEventHandler::HandleEntityEventPostProcess(world, entityList->Get(i));

		if (entityList->Get(i)->GetProcessEventQueue()->Size() > 0)
			LMEntityProcessEventHandler::HandleEntityEventPostProcess(world, entityList->Get(i));
	}*/

	return 0;
}

int EntityManager::HandleEntitySelection(EntitySystem* entitySystem)
{
	//_entitySelectionHandler->CalcMouseOver(world);

	return 0;
}

int EntityManager::SetOrthoSquare(EntitySystem* entitySystem, glm::vec4* points)
{
	//_entitySelectionHandler->SetOrthoSquare(world, points);

	return 0;
}

int EntityManager::SelectMouseOver(EntitySystem* entitySystem)
{
	//_entitySelectionHandler->SelectMouseOver(world);

	return 0;
}
