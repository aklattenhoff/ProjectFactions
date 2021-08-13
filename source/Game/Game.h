#ifndef _GAME_H
#define _GAME_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_thread.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/World.h"
#include "Game/Input/Input.h"

//#include "Graphics/Camera.h"
//#include "Game/Terrain/Terrain.h"
#include "Game/PathFinder/PathFinder.h"
//#include "Game/Terrain/TerrainPainter.h"
#include "Phage/Phage_Terrain.h"
#include "Game/LOSMap/LOSMap.h"
#include "Game/LOS/LOS.h"
//#include "Game/Bloom/Bloom.h"
#include "Game/GUI/GUIManager.h"
#include "Game/ParticleSystem/ParticleSystem.h"
//#include "Game/ShadowMap/ShadowMap.h"

#include "Game/Faction/FactionBaseList.h"
#include "Game/ActionManager/ActionManager.h"
#include "Game/EntityManager/EntityManager.h"
#include "Game/EntityManager/EntitySelectionList.h"
#include "Game/EntityManager/EntityMouseOverList.h"
#include "Game/EntityMovement/EntityMovement.h"
#include "Game/EntityMovement/EntityCollision.h"
#include "Game/ActionManager/ActionProgressQueue.h"
#include "Game/EntityManager/EntityMap.h"
#include "Game/Entity/EntityList.h"
#include "Game/Player/PlayerList.h"
#include "Game/SquadronManager/SquadronManager.h"
#include "Game/ProjectileSystem/ProjectileSystem.h"

#include "Network/Network.h"
class Parser;
class EntityBaseList;
class TempEntityBaseListCreator;

class ActionList;
class ActionListCreator;

//Main class and entry point for a new game
class Game
{
public:
	Game();
	~Game();

	//Sets screen resolution for viewport
	int InitCamera(glm::vec2 screenResolution);

	//Initializes all the system objects for the game
	int Init();

	//Sets the window and context
	int SetWindow(SDL_Window* w);

	//Sets the list of factions parsed from the faction list xml
	int SetFactionList(FactionList* fList);

	int SetParser(Parser* parser);

	//Parses the faction list and all base components for each faction (Entity, Actions, etc.)
	int ParseFactions(const char* fileName);

	//Sets the file for the bottom bar xml
	int SetBottomBarFile(const char* bottomBar);

	//Creates a new game and all new game objects (e.g. players, entities, etc.)
	int CreateNewGame();

	//Join an existing game and retrieve data from host
	//NIY: This will be implemented with net library
	int JoinExistingGame();

	//Begins the game loop
	int Run();
private:
	//One iteration of the game loop
	int Frame();
	//Populates Input object for current frame
	int HandleInput();
	//Handles orbiting of the camera
	int OrbitCamera();
private:
	//Passed in variables
	SDL_Window* window;

	//Camera* camera;
	SDL_Event sdlEvent;

	World* world;
	Input* input;

	//Managers
	GUIManager* guiManager;
	ActionManager* actionManager;
	EntityManager* entityManager;
	SquadronManager* _squadronManager;

	//Extra systems
	ProjectileSystem* _projectileSystem;

	//Passed in by the Factions object
	//NOTE: Do not delete. The Factions object deletes this
	FactionList* factionList;
	
	FactionBaseList* _factionBaseList;

	EntityList* _entityList;
	EntityMap* _entityMap;
	PlayerList* _playerList;
	EntitySelectionList* _entitySelectionList;
	EntityMouseOverList* _entityMouseOverList;
	EntityMovement* _entityMovement;
	EntityCollision* _entityCollision;
	ActionProgressQueue* _actionProgressQueue;

	//Terrain* terrain;
	PathFinder* _pathFinder;
	//TerrainPainter* terrainPainter;
	Phage::PhageTerrain* _phageTerrain;

	LOSMap* losMap;
	LOS* _los;
	//Bloom* _bloom;
	ParticleSystem* _particleSystem;
	//ShadowMap* _shadowMap;

	const char* bottomBarFileName;

	float rotate;
	float orbitAmount;
	float orbitAmountY;
	float orbitX;
	float orbitY;
	float orbitMagnitude;
	float orbitBaseRotation;

	//FPS
	unsigned int ticks;
	float FPS;

	bool mouseButtonLeftDown;
	glm::vec2 mouse1;
	glm::vec2 mouse2;
	bool mouseMotion;

	int _gameSpeed;

	//Debugging keyboard short-cuts
	bool boundingBoxOn;
	bool terrainPaintOn;
	bool terrainMapOn;
	int terrainPaintValue;

	bool mouseOverGUI;
	bool _mouseUsedByActionManager;

	Network* _network;
	Parser* _parser;
	EntityBaseList* _entityBaseList;
	TempEntityBaseListCreator* _tempEntityBaseListCreator;

	ActionList* _actionList;
	ActionListCreator* _actionListCreator;

	//Phage::PhageEntity* _phageEntity;
	Texturer::Texture _textureCursor;
	RS::GUITexture _rsGUITextureCursor;
};

#endif
