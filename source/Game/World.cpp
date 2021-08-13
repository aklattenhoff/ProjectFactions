#include "World.h"

//#include "Graphics/Camera.h"
//#include "Game/Terrain/Terrain.h"
#include "Game/Input/Input.h"
#include "Game/LOSMap/LOSMap.h"

#include "Game/GUI/GUIManager.h"
#include "Game/ActionManager/ActionManager.h"

#include "Game/Entity/EntityBaseList.h"
#include "Game/ActionManager/ActionList.h"

#include "Game/ProjectileSystem/ProjectileSystem.h"

#include "Phage/Phage_Terrain.h"

World::World()
{
}

World::~World()
{
}

/*int World::SetCamera(Camera* c)
{
	_camera = c;

	return 0;
}*/

int World::SetTerrain(Terrain* t)
{
	_terrain = t;

	return 0;
}

int World::SetPathFinder(PathFinder* p)
{
	_pathFinder = p;

	return 0;
}

int World::SetInput(Input* i)
{
	_input = i;

	return 0;
}

int World::SetLOSMap(LOSMap* l)
{
	_losMap = l;

	return 0;
}

int World::SetLOS(LOS* l)
{
	_los = l;

	return 0;
}

int World::SetGUIManager(GUIManager* g)
{
	_guiManager = g;

	return 0;
}

int World::SetActionManager(ActionManager* a)
{
	_actionManager = a;

	return 0;
}

int World::SetEntityManager(EntityManager* e)
{
	_entityManager = e;

	return 0;
}

int World::SetSquadronManager(SquadronManager* e)
{
	_squadronManager = e;

	return 0;
}

int World::SetEntityMovement(EntityMovement* e)
{
	_entityMovement = e;

	return 0;
}

int World::SetEntityCollision(EntityCollision* e)
{
	_entityCollision = e;

	return 0;
}

int World::SetFactionBaseList(FactionBaseList* e)
{
	_factionBaseList = e;

	return 0;
}

int World::SetFactionList(FactionList* e)
{
	_factionList = e;

	return 0;
}

int World::SetEntityList(EntityList* e)
{
	_entityList = e;

	return 0;
}

int World::SetEntityMap(EntityMap* e)
{
	_entityMap = e;

	return 0;
}

int World::SetPlayerList(PlayerList* p)
{
	_playerList = p;

	return 0;
}

int World::SetEntitySelectionList(EntitySelectionList* e)
{
	_entitySelectionList = e;

	return 0;
}

int World::SetEntityMouseOverList(EntityMouseOverList* e)
{
	_entityMouseOverList = e;

	return 0;
}

int World::SetActionProgressQueue(ActionProgressQueue* a)
{
	_actionProgressQueue = a;

	return 0;
}

int World::SetEntityBaseList(EntityBaseList* a)
{
	_entityBaseList = a;

	return 0;
}

int World::SetActionList(ActionList* a)
{
	_actionList = a;

	return 0;
}

int World::SetProjectileSystem(ProjectileSystem* p)
{
	_projectileSystem = p;

	return 0;
}

int World::SetPhageTerrain(Phage::PhageTerrain* p)
{
	_phageTerrain = p;

	return 0;
}

int World::SetFrameNumber(int f)
{
	_frameNumber = f;

	return 0;
}

int World::SetCreateTime(unsigned int t)
{
	_createTime = t;

	_gameTime = 0;

	return 0;
}

int World::SetGameTime(unsigned int t)
{
	_deltaTime = t - _gameTime;
	_deltaSeconds = ((float)_deltaTime) / 1000.0f;

	_gameTime = t;

	return 0;
}


/*Camera* World::GetCamera()
{
	return _camera;
}*/

Terrain* World::GetTerrain()
{
	return _terrain;
}

PathFinder* World::GetPathFinder()
{
	return _pathFinder;
}

Input* World::GetInput()
{
	return _input;
}

LOSMap* World::GetLOSMap()
{
	return _losMap;
}

LOS* World::GetLOS()
{
	return _los;
}

GUIManager* World::GetGUIManager()
{
	return _guiManager;
}

ActionManager* World::GetActionManager()
{
	return _actionManager;
}

EntityManager* World::GetEntityManager()
{
	return _entityManager;
}

SquadronManager* World::GetSquadronManager()
{
	return _squadronManager;
}

EntityMovement* World::GetEntityMovement()
{
	return _entityMovement;
}

EntityCollision* World::GetEntityCollision()
{
	return _entityCollision;
}

FactionList* World::GetFactionList()
{
	return _factionList;
}

FactionBaseList* World::GetFactionBaseList()
{
	return _factionBaseList;
}

EntityList* World::GetEntityList()
{
	return _entityList;
}

EntityMap* World::GetEntityMap()
{
	return _entityMap;
}

PlayerList* World::GetPlayerList()
{	
	return _playerList;
}

EntitySelectionList* World::GetEntitySelectionList()
{
	return _entitySelectionList;
}

EntityMouseOverList* World::GetEntityMouseOverList()
{
	return _entityMouseOverList;
}

ActionProgressQueue* World::GetActionProgressQueue()
{
	return _actionProgressQueue;
}

EntityBaseList* World::GetEntityBaseList()
{
	return _entityBaseList;
}

ActionList* World::GetActionList()
{
	return _actionList;
}

ProjectileSystem* World::GetProjectileSystem()
{
	return _projectileSystem;
}

int World::GetFrameNumber()
{
	return _frameNumber;
}

unsigned int World::GetCreateTime()
{
	return _createTime;
}

unsigned int World::GetGameTime()
{
	return _gameTime;
}

unsigned int World::GetDeltaTime()
{
	return _deltaTime;
}

float World::GetDeltaSeconds()
{
	return _deltaSeconds;
}

ActionManagerInterface* World::GetActionManagerInterface()
{
	return _actionManager->GetInterface();
}

Phage::PhageTerrain* World::GetPhageTerrain()
{
	return _phageTerrain;
}

bool World::IsMouseOverGUI()
{
	return _guiManager->IsMouseOverGUI();
}
