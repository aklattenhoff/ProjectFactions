#include "EntitySystem.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityBaseListCreator.h"
#include "Game/EntitySystem/EntityManager.h"
//#include "Game/EntitySystem/EntityRenderer.h"
//#include "Game/EntitySystem/DebugEntityRenderer.h"

#include "Game/EntitySystem/EntityBaseList.h"
#include "Game/EntitySystem/EntityList.h"

#include "Parser/Parser.h"

EntitySystem::EntitySystem()
{
	_entityBaseListCreator = NULL;
	_entityManager = NULL;
	_entityRenderer = NULL;
	_debugEntityRenderer = NULL;

	_entityBaseList = NULL;
	_entityList = NULL;
}

EntitySystem::~EntitySystem()
{
	delete _entityBaseListCreator;
	delete _entityManager;
	delete _entityRenderer;
	delete _debugEntityRenderer;

	delete _entityBaseList;
	delete _entityList;
}

int EntitySystem::CreateEntitySystem()
{
	_entityBaseListCreator = new EntityBaseListCreator();
	_entityBaseListCreator->Init();

	_entityManager = new EntityManager();
	_entityManager->Init();

	//_entityRenderer = new EntityRenderer();
	//_entityRenderer->Init();

	//_debugEntityRenderer = new DebugEntityRenderer();
	//_debugEntityRenderer->Init();

	//Do not create the entity base list!!!
	//NOTE: The entity base list creator will do this
	//_entityBaseList = new EntityBaseList();

	_entityList = new EntityList();

	return 0;
}

int EntitySystem::CreateEntityBaseList(Parser* parser)
{
	_entityBaseList = _entityBaseListCreator->CreateEntityBaseList(parser);

	return 0;
}

EntityManager* EntitySystem::GetEntityManager()
{
	return _entityManager;
}

EntityRenderer* EntitySystem::GetEntityRenderer()
{
	return _entityRenderer;
}

DebugEntityRenderer* EntitySystem::GetDebugEntityRenderer()
{
	return _debugEntityRenderer;
}

EntityBaseList* EntitySystem::GetEntityBaseList()
{
	return _entityBaseList;
}

EntityList* EntitySystem::GetEntityList()
{
	return _entityList;
}
