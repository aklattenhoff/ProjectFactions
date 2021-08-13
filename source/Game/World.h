#ifndef _WORLD_H
#define _WORLD_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera;
class Terrain;
class PathFinder;
class Input;
class LOSMap;
class LOS;

class GUIManager;
class ActionManager;
class EntityManager;
class SquadronManager;

class EntityMovement;
class EntityCollision;

//PlayerList
class FactionList;
class FactionBaseList;
class EntityList;
class EntityMap;
class PlayerList;
class EntitySelectionList;
class EntityMouseOverList;
class ActionProgressQueue;

class EntityBaseList;
class ActionList;

class ProjectileSystem;

//Classes that aren't set directly in the world

//Interfaces
class ActionManagerInterface;

//Phage
namespace Phage
{
	class PhageTerrain;
}

class World
{
public:
	World();
	~World();

	//Set functions
	//int SetCamera(Camera* c);
	int SetTerrain(Terrain* t);
	int SetPathFinder(PathFinder* p);
	int SetInput(Input* i);
	int SetLOSMap(LOSMap* l);
	int SetLOS(LOS* l);

	int SetGUIManager(GUIManager* g);
	int SetActionManager(ActionManager* a);
	int SetEntityManager(EntityManager* e);
	int SetSquadronManager(SquadronManager* e);

	int SetEntityMovement(EntityMovement* e);
	int SetEntityCollision(EntityCollision* e);

	//PlayerList
	int SetFactionList(FactionList* e);
	int SetFactionBaseList(FactionBaseList* e);
	int SetEntityList(EntityList* e);
	int SetEntityMap(EntityMap* e);
	int SetPlayerList(PlayerList* p);
	int SetEntitySelectionList(EntitySelectionList* e);
	int SetEntityMouseOverList(EntityMouseOverList* e);
	int SetActionProgressQueue(ActionProgressQueue* a);

	int SetEntityBaseList(EntityBaseList* a);
	int SetActionList(ActionList* a);

	int SetProjectileSystem(ProjectileSystem* p);

	int SetPhageTerrain(Phage::PhageTerrain* p);

	//FrameNumber
	int SetFrameNumber(int f);

	int SetCreateTime(unsigned int t);
	int SetGameTime(unsigned int t);

	//Get functions
	//Camera* GetCamera();
	Terrain* GetTerrain();
	PathFinder* GetPathFinder();
	Input* GetInput();
	LOSMap* GetLOSMap();
	LOS* GetLOS();

	GUIManager* GetGUIManager();
	ActionManager* GetActionManager();
	EntityManager* GetEntityManager();
	SquadronManager* GetSquadronManager();

	EntityMovement* GetEntityMovement();
	EntityCollision* GetEntityCollision();

	//PlayerList
	FactionList* GetFactionList();
	FactionBaseList* GetFactionBaseList();
	EntityList* GetEntityList();
	EntityMap* GetEntityMap();
	PlayerList* GetPlayerList();
	EntitySelectionList* GetEntitySelectionList();
	EntityMouseOverList* GetEntityMouseOverList();
	ActionProgressQueue* GetActionProgressQueue();

	EntityBaseList* GetEntityBaseList();
	ActionList* GetActionList();

	ProjectileSystem* GetProjectileSystem();

	//FrameNumber
	int GetFrameNumber();

	unsigned int GetCreateTime();
	unsigned int GetGameTime();
	unsigned int GetDeltaTime();
	float GetDeltaSeconds();

	//Dynamic get functions
	ActionManagerInterface* GetActionManagerInterface();

	Phage::PhageTerrain* GetPhageTerrain();

	bool IsMouseOverGUI();
private:
	//Camera* _camera;
	Terrain* _terrain;
	PathFinder* _pathFinder;
	Input* _input;
	LOSMap* _losMap;
	LOS* _los;

	GUIManager* _guiManager;
	ActionManager* _actionManager;
	EntityManager* _entityManager;
	SquadronManager* _squadronManager;

	EntityMovement* _entityMovement;
	EntityCollision* _entityCollision;

	FactionList* _factionList;
	FactionBaseList* _factionBaseList;
	EntityList* _entityList;
	EntityMap* _entityMap;
	PlayerList* _playerList;
	EntitySelectionList* _entitySelectionList;
	EntityMouseOverList* _entityMouseOverList;
	ActionProgressQueue* _actionProgressQueue;

	EntityBaseList* _entityBaseList;
	ActionList* _actionList;

	ProjectileSystem* _projectileSystem;

	Phage::PhageTerrain* _phageTerrain;

	int _frameNumber;

	unsigned int _createTime;
	unsigned int _gameTime;
	unsigned int _deltaTime;
	float _deltaSeconds;
};

#endif
