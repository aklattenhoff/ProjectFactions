#include "Game.h"

#include "Parser/Parser.h"
#include "Game/Entity/EntityBaseList.h"
#include "Game/TempEntityCreator/TempEntityBaseListCreator.h"

#include "Game/ActionManager/ActionListCreator.h"
#include "Game/ActionManager/ActionList.h"

#include "Game/Player/Player.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/GUIToolTipBackground.h"
#include "Renderer/RS/PhageTerrainDecal.h"

#include "Phage/Phage.h"
#include "Game/Entity/EntityProcessEvent.h"
#include "Game/Entity/EntityProcessEventQueue.h"

Game::Game()
{
	//camera = NULL;
	world = NULL;
	input = NULL;
	entityManager = NULL;
	guiManager = NULL;
	//terrain = NULL;
	_pathFinder = NULL;
	//terrainPainter = NULL;
	_phageTerrain = NULL;
	losMap = NULL;
	_los = NULL;
	//_bloom = NULL;
	_particleSystem = NULL;
	//_shadowMap = NULL;

	mouseButtonLeftDown = false;
	mouse1 = glm::vec2(0.0f, 0.0f);
	mouse2 = glm::vec2(0.0f, 0.0f);

	_gameSpeed = 1;

	boundingBoxOn = false;
	terrainPaintOn = false;
	terrainMapOn = false;
	terrainPaintValue = 0;

	mouseOverGUI = false;
	actionManager = NULL;
	_squadronManager = NULL;

	_projectileSystem = NULL;

	_factionBaseList = NULL;
	_entityList = NULL;
	_entityMap = NULL;
	_playerList = NULL;
	_entitySelectionList = NULL;
	_entityMouseOverList = NULL;
	_entityMovement = NULL;
	_entityCollision = NULL;
	_actionProgressQueue = NULL;

	//_network = NULL;

	//Do not delete parser
	_parser = NULL;

	_entityBaseList = NULL;
	_tempEntityBaseListCreator = NULL;
	_actionList = NULL;
	_actionListCreator = NULL;
}

Game::~Game()
{
	//delete camera;
	delete world;
	delete input;
	delete entityManager;
	delete guiManager;
	//delete terrain;
	delete _pathFinder;
	//delete terrainPainter;
	delete _phageTerrain;
	delete losMap;
	delete _los;
	//delete _bloom;
	delete actionManager;
	delete _squadronManager;
	delete _projectileSystem;
	delete _particleSystem;
	//delete _shadowMap;

	delete _factionBaseList;
	delete _entityList;
	delete _entityMap;
	delete _playerList;
	delete _entitySelectionList;
	delete _entityMouseOverList;
	delete _entityMovement;
	delete _entityCollision;
	delete _actionProgressQueue;

	//delete _network;
	delete _entityBaseList;
	delete _tempEntityBaseListCreator;
	delete _actionList;
	delete _actionListCreator;
}

int Game::InitCamera(glm::vec2 screenResolution)
{
	//Creates the Camera object and passes the screen resolution
	//camera = new Camera();
	//camera->SetScreenResolution(screenResolution);

	//camera->setCameraPosition(11.0f, 32.0f, 44.0f);
	rotate = 0.0f;

	return 0;
}

int Game::Init()
{
	//Creates objects for the game
	world = new World();

	//User input manager
	input = new Input();
	input->Init();

	//Managers
	guiManager = new GUIManager();
	guiManager->Init();
	//guiManager->PreRenderGUI(camera->GetScreenResolution());

	actionManager = new ActionManager();
	actionManager->Init();

	entityManager = new EntityManager();
	entityManager->Init();

	_squadronManager = new SquadronManager();
	_squadronManager->Init();

	_projectileSystem = new ProjectileSystem();
	_projectileSystem->Init();

	//Initialize the terrain
	//terrain = new Terrain();
	//terrain->Init();

	_phageTerrain = new Phage::PhageTerrain();
	_phageTerrain->Init();
	_phageTerrain->CreateTerrain(256, 256, 8);
	//_phageTerrain->CreateAndAddTerrainDecal("models/market_decal.png", 10.0f, 10.0f, 45.0f);

	_pathFinder = new PathFinder();
	_pathFinder->Init();
	_pathFinder->CreateObstacleMap(_phageTerrain->GetTerrainSize().x, _phageTerrain->GetTerrainSize().y, 2);

	_textureCursor.file = "art/cursor.png";
	_textureCursor.yflip = false;
	_textureCursor.mipmap = false;
	TexturerSingleton->LoadTexture(&_textureCursor, 1);

	//terrainPainter = new TerrainPainter();
	//terrainPainter->Init();
	//terrainPainter->SetLength(1);

	losMap = new LOSMap();
	//losMap->Init(terrain->getTerrainGrid()->getCellX(), terrain->getTerrainGrid()->getCellY(), 1);
	losMap->Init((int)_phageTerrain->GetTerrainSize().x, (int)_phageTerrain->GetMouseOnTerrain().y, 1);

	_los = new LOS();
	_los->Init(256, 256);

	//_bloom = new Bloom();
	//_bloom->Init();

	_particleSystem = new ParticleSystem();
	_particleSystem->Init();

	//_shadowMap = new ShadowMap();
	//Init is called after world is set up

	_factionBaseList = new FactionBaseList();
	_factionBaseList->Init();

	_entityList = new EntityList();

	_entityMap = new EntityMap();
	//_entityMap->Init(terrain->getTerrainGrid()->getCellX(), terrain->getTerrainGrid()->getCellY(), 2);
	_entityMap->Init(_phageTerrain->GetTerrainSize().x, _phageTerrain->GetTerrainSize().y, 2);

	_playerList = new PlayerList();
	_playerList->Init();

	_entitySelectionList = new EntitySelectionList();
	_entitySelectionList->Init();

	_entityMouseOverList = new EntityMouseOverList();
	_entityMouseOverList->Init();

	_entityMovement = new EntityMovement();
	_entityMovement->Init();

	_entityCollision = new EntityCollision();
	_entityCollision->Init();

	_actionProgressQueue = new ActionProgressQueue();
	_actionProgressQueue->Init();

	ticks = SDL_GetTicks();

	/*//Push what we can into the world object
	//The camera should have been created at this point
	world->SetCamera(camera);
	world->SetInput(input);
	world->SetTerrain(terrain);
	world->SetLOSMap(losMap);

	//Push the managers into the world object
	//NOTE: The objects owned by managers that are loaded later (e.g. ObjectInstanceArray)
	//are accessed dynamically when a get function is called in the world. So, we don't have
	//to worry about setting the managers after they have created new instances of those classes
	world->SetGUIManager(guiManager);
	world->SetActionManager(actionManager);
	world->SetObjectManager(objectManager);
	world->SetEntityManager(entityManager);
	
	//Set PlayerList
	world->SetEntityList(_entityList);
	world->SetEntityMap(_entityMap);
	world->SetPlayerList(_playerList);
	world->SetEntitySelectionList(_entitySelectionList);
	world->SetEntityMouseOverList(_entityMouseOverList);*/

	//_network = new Network();

	return 0;
}

int Game::SetWindow(SDL_Window* w)
{
	window = w;

	return 0;
}

int Game::SetFactionList(FactionList* fList)
{
	factionList = fList;

	return 0;
}

int Game::SetParser(Parser* parser)
{
	_parser = parser;

	return 0;
}

int Game::ParseFactions(const char* fileName)
{
	//_factionBaseList->CreateAllFactions(fileName);

	_tempEntityBaseListCreator = new TempEntityBaseListCreator();
	_tempEntityBaseListCreator->Init();
	_entityBaseList = _tempEntityBaseListCreator->CreateEntityBaseList(_parser);

	_actionListCreator = new ActionListCreator();
	_actionListCreator->Init();
	_actionList = _actionListCreator->CreateActionList(_parser);

	return 0;
}

int Game::SetBottomBarFile(const char* bottomBar)
{
	bottomBarFileName = bottomBar;

	guiManager->SetBottomBarFile(bottomBarFileName);

	return 0;
}

int Game::CreateNewGame()
{
	//guiManager->PreRenderGUI(camera->GetScreenResolution());
	guiManager->PreRenderGUI(glm::vec2(RendererSingleton->GetScreenWidth(), RendererSingleton->GetScreenHeight()));

	//Push what we can into the world object
	//The camera should have been created at this point
	//world->SetCamera(camera);
	world->SetInput(input);
	//world->SetTerrain(terrain);
	world->SetPathFinder(_pathFinder);
	world->SetLOSMap(losMap);
	world->SetLOS(_los);

	//Push the managers into the world object
	//NOTE: The objects owned by managers that are loaded later (e.g. ObjectInstanceArray)
	//are accessed dynamically when a get function is called in the world. So, we don't have
	//to worry about setting the managers after they have created new instances of those classes
	world->SetGUIManager(guiManager);
	world->SetActionManager(actionManager);
	world->SetEntityManager(entityManager);
	world->SetSquadronManager(_squadronManager);

	world->SetProjectileSystem(_projectileSystem);
	
	//Set PlayerList
	world->SetFactionList(factionList);
	world->SetFactionBaseList(_factionBaseList);
	world->SetEntityList(_entityList);
	world->SetEntityMap(_entityMap);
	world->SetPlayerList(_playerList);
	world->SetEntitySelectionList(_entitySelectionList);
	world->SetEntityMouseOverList(_entityMouseOverList);
	world->SetEntityMovement(_entityMovement);
	world->SetEntityCollision(_entityCollision);
	world->SetActionProgressQueue(_actionProgressQueue);
	world->SetEntityBaseList(_entityBaseList);
	world->SetActionList(_actionList);

	world->SetPhageTerrain(_phageTerrain);

	//Create Gaia at 0 and then other players starting at 1
	_playerList->CreateAndAdd();
	_playerList->GetAt(0)->SetColor(0.0f, 1.0f, 0.0f);
	_playerList->GetAt(0)->SetTeam(0);

	_playerList->CreateAndAdd();
	_playerList->GetAt(1)->SetColor(0.0f, 0.0f, 1.0f);
	_playerList->GetAt(1)->SetTeam(1);

	_playerList->CreateAndAdd();
	_playerList->GetAt(2)->SetColor(1.0f, 0.0f, 0.0f);
	_playerList->GetAt(2)->SetTeam(2);

	_playerList->SetGameOwner(1);

	RendererSingleton->SetGameSpeed(_gameSpeed);

	//_shadowMap->Init(world);

	//TEST: Create initial instances
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_mill")->CreateNewInstance(8.0f, 17.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		//Entity* e = _entityBaseList->At("kreytons_mill")->CreateNewInstance(12.0f, 17.0f, 0.0f, 0);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_faction_center")->CreateNewInstance(18.0f, 8.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		Entity* e = _entityBaseList->At("kreytons_faction_center")->CreateNewInstance(18.0f+50.0f, 12.0f+50.0f, 0.0f, 1, _phageTerrain->CalculateHeight(18.0f, 18.0f));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);

		EntityProcessEvent* processEventToEnqueue = new EntityProcessEvent();
		processEventToEnqueue->eventType = EntityProcessEvent::TRAIN_ENTITY;
		processEventToEnqueue->owner = e;
		processEventToEnqueue->entityBase = world->GetEntityBaseList()->At("kreytons_villager");
		processEventToEnqueue->trainPoints = 0;
		e->GetProcessEventQueue()->Enqueue(processEventToEnqueue);
	}
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_armory")->CreateNewInstance(24.0f, 17.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		//Entity* e = _entityBaseList->At("kreytons_armory")->CreateNewInstance(24.0f, 17.0f, 0.0f, 0);
		//e->AddCurrentBuildPoints(25.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}


	for (int i = 0;i < 100;i ++)
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_villager")->CreateNewInstance(rand()%100/10.0f, rand()%100/10.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		float vilX = rand()%100/10.0f+3.0f+50.0f;
		float vilY = rand()%100/10.0f+3.0f+50.0f;
		Entity* e = _entityBaseList->At("kreytons_villager")->CreateNewInstance(vilX, vilY, 0.0f, 1, _phageTerrain->CalculateHeight(vilX, vilY));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
	}

	for (int i = 0; i < 50; i++)
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_tree")->CreateNewInstance(5.0f, 17.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		glm::vec2 pos;
		pos.x = 30.0f + rand() % 200 / 10.0f + 50.0f;
		pos.y = 0.0f + rand() % 200 / 10.0f + 50.0f;
		Entity* e = _entityBaseList->At("kreytons_tree_new")->CreateNewInstance(pos.x, pos.y, (float)(rand() % 360), 0, _phageTerrain->CalculateHeight(pos.x, pos.y));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
	}

	{
		Entity* e = _entityBaseList->At("kreytons_deer")->CreateNewInstance(50.0f, 50.0f, (float)(rand() % 360), 0, _phageTerrain->CalculateHeight(20.0f, 20.0f));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
	}

	{
		//Entity* e = _entityBaseList->At("kreytons_swordsman")->CreateNewInstance(18.0f, 20.0f, 0.0f, 0);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}

	{
		//Entity* e = _entityBaseList->At("kreytons_swordsman")->CreateNewInstance(20.0f, 20.0f, 0.0f, 1);
		//_playerList->GetAt(1)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}

	//for (int i = 0;i < 10;i ++)
	{
		//Entity* e = _entityBaseList->At("kreytons_archer")->CreateNewInstance(17.0f+i, 23.0f, 0.0f, 0);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}

	{
		//Entity* e = _entityBaseList->At("kreytons_archer")->CreateNewInstance(20.0f, 22.0f, 0.0f, 1);
		//_playerList->GetAt(1)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}

	for (int i = 0;i < 30;i ++)
	{
		//Entity* e = _factionBaseList->At("Inferno Horde")->GetEntityBaseList()->At("kreytons_tree")->CreateNewInstance(5.0f, 17.0f, 0.0f);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);

		glm::vec2 pos;
		pos.x = 20.0f+rand()%100/10.0f+50.0f;
		pos.y = 20.0f+rand()%100/10.0f+50.0f;
		Entity* e;
		int choice = rand()%3;
		if (choice == 0) e = _entityBaseList->At("kreytons_tree")->CreateNewInstance(pos.x, pos.y, (float)(rand() % 360), 0, _phageTerrain->CalculateHeight(pos.x, pos.y));
		else if (choice == 1) e = _entityBaseList->At("kreytons_tree_b")->CreateNewInstance(pos.x, pos.y, (float)(rand() % 360), 0, _phageTerrain->CalculateHeight(pos.x, pos.y));
		else e = _entityBaseList->At("kreytons_tree_c")->CreateNewInstance(pos.x, pos.y, (float)(rand() % 360), 0, _phageTerrain->CalculateHeight(pos.x, pos.y));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		glm::ivec2 ipos;
		ipos.x = (int)floor(pos.x+0.5f);
		ipos.y = (int)floor(pos.y+0.5f);
		//for (int ty = -2;ty < 2;ty ++)
			//for (int tx = -2;tx < 2;tx ++)
				//_phageTerrain->PaintPosition(ipos.x+tx, ipos.y+ty, 6);
		//TerrainMapCellsVector terrainMapCellsVec;
		//for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			//terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		//Entity* e = _entityBaseList->At("kreytons_quarry")->CreateNewInstance(10.0f+3.0f, 23.0f+4.0f, 0.0f, 0);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}
	{
		Entity* e = _entityBaseList->At("kreytons_quarry_1")->CreateNewInstance(10.0f+3.0f+50.0f, 23.0f+4.0f+50.0f, 0.0f, 0, _phageTerrain->CalculateHeight(13.0f, 27.0f));
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
	}
	{
		//Entity* e = _entityBaseList->At("kreytons_granary")->CreateNewInstance(24.0f, 20.0f, 0.0f, 0);
		//_playerList->GetAt(0)->GetEntityList()->Add(e);
		//_entityList->Add(e);
		//_entityMap->Add(e);
	}
	/*{
		Entity* e = _entityBaseList->At("kreytons_market")->CreateNewInstance(24.0f, 26.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		_phageTerrain->CreateAndAddTerrainDecal("models/market_decal.png", 24.0f, 26.0f, 0.0f);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_market")->CreateNewInstance(34.0f, 26.0f, 45.0f, 1);
		_playerList->GetAt(1)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		_phageTerrain->CreateAndAddTerrainDecal("models/market_decal.png", 34.0f, 26.0f, 45.0f);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}

	{
		Entity* e = _entityBaseList->At("kreytons_outpost")->CreateNewInstance(24.0f, 35.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}

	{
		Entity* e = _entityBaseList->At("kreytons_wall_connector")->CreateNewInstance(20.0f, 40.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_wall_4x1")->CreateNewInstance(22.0f, 40.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_wall_connector")->CreateNewInstance(24.0f, 40.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_agronome_guild")->CreateNewInstance(34.0f, 35.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_manor")->CreateNewInstance(30.0f, 7.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}
	{
		Entity* e = _entityBaseList->At("kreytons_barracks")->CreateNewInstance(36.0f, 7.0f, 0.0f, 0);
		_playerList->GetAt(0)->GetEntityList()->Add(e);
		_entityList->Add(e);
		_entityMap->Add(e);
		TerrainMapCellsVector terrainMapCellsVec;
		for (int i = 0;i < e->GetEntityBase()->BoundingBox2D()->Size();i ++)
			terrain->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i), e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
	}*/

	//BoundingBox2DSet bb2DSet;
	//bb2DSet.AddBoundingBox(glm::vec2(-2.5f, 0.0f), glm::vec2(2.5f, 0.0f), 5.0f);
	//TerrainMapCellsVector tmcv;
	//world->GetTerrain()->getTerrainMap()->SetRectangle(bb2DSet.Get(0), glm::vec3(10.0f, 0.0f, 15.0f), 0.0f, MAP_BLOCKED, &tmcv);

	//Create all other players. This needs to be defined somewhere through the initial game setup UI
	//TODO: Create AI and other net players

	//_network->Init();

	//LOS::LOSPoints points;

	//Load text icons
	char textIconChar[] = {'f', 'w'};
	std::string textIconStr[] = {"textures/ui/food.png", "textures/ui/wood.png"};
	for (int i = 0;i < 2;i ++)
	{
		SDL_Surface *surface = NULL;
		surface = IMG_Load(textIconStr[i].c_str());
		if (!surface) return -1;

		GLenum textureFormat;
		if (surface->format->BytesPerPixel == 4)
			textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
		else if (surface->format->BytesPerPixel == 3)
			textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

		GLuint iconTex;
		glGenTextures(1, &iconTex);
		glBindTexture(GL_TEXTURE_2D, iconTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(surface);

		RendererSingleton->AddPhageUITextIcon(textIconChar[i], iconTex);
	}


	RS::GUIToolTipBackground rsBorderBack;
	RS::GUIToolTipBackground::BorderBackground borderBack;
	for (int i = 0;i < 9;i ++)
	{
		SDL_Surface *surface = NULL;
		if (i == 0) surface = IMG_Load("textures/ui/tooltip.png");
		else if (i == 1) surface = IMG_Load("textures/ui/tooltip_NW.png");
		else if (i == 2) surface = IMG_Load("textures/ui/tooltip_NE.png");
		else if (i == 3) surface = IMG_Load("textures/ui/tooltip_SE.png");
		else if (i == 4) surface = IMG_Load("textures/ui/tooltip_SW.png");
		else if (i == 5) surface = IMG_Load("textures/ui/tooltip_N.png");
		else if (i == 6) surface = IMG_Load("textures/ui/tooltip_S.png");
		else if (i == 7) surface = IMG_Load("textures/ui/tooltip_E.png");
		else if (i == 8) surface = IMG_Load("textures/ui/tooltip_W.png");
		if (!surface) return -1;

		GLenum textureFormat;
		if (surface->format->BytesPerPixel == 4)
			textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
		else if (surface->format->BytesPerPixel == 3)
			textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

		GLuint iconTex;
		glGenTextures(1, &iconTex);
		glBindTexture(GL_TEXTURE_2D, iconTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(surface);

		if (i == 0) borderBack._backTex = iconTex;
		else if (i == 1) borderBack._cornerTexNW = iconTex;
		else if (i == 2) borderBack._cornerTexNE = iconTex;
		else if (i == 3) borderBack._cornerTexSE = iconTex;
		else if (i == 4) borderBack._cornerTexSW = iconTex;
		else if (i == 5) borderBack._edgeTexN = iconTex;
		else if (i == 6) borderBack._edgeTexS = iconTex;
		else if (i == 7) borderBack._edgeTexE = iconTex;
		else if (i == 8) borderBack._edgeTexW = iconTex;
	}
	rsBorderBack.SetToolTipBackgroundTextures(borderBack);
	rsBorderBack.SetBorderWidth(4);
	RendererSingleton->SetGUIToolTipBackground(rsBorderBack);

	Phage::PhageSound* mainMusic = new Phage::PhageSound();
	mainMusic->Init("music/resilience.ogg");
	//RendererSingleton->SetBackgroundMusic(mainMusic);

	return 0;
}

bool rt_loop = false;
bool rt_end = false;

int RenderThread(void* data)
{
	bool done = false;
	while(!done)
	{
		if (rt_end)
			done = true;
		if (rt_loop)
		{
			((World*)data)->GetEntityManager()->Draw(((World*)data));
			//RendererSingleton->Render();
			//DebugOutput("Thread is running\n");
			rt_loop = false;
		}
		SDL_Delay(1);
	}

	return 0;
}

int Game::Run()
{
	SDL_ShowCursor(0);

	int returnValue = 0;

	//SDL_Thread* thread = SDL_CreateThread(RenderThread, "RenderThread", (void*)world);

	world->SetCreateTime(SDL_GetTicks());

	//Main game loop
	bool done = false;
	while (!done)
	{
		world->SetGameTime(SDL_GetTicks() - world->GetCreateTime());
		//RendererSingleton->AddPhageEntityToScene(_phageEntity);
		//rt_loop = true;
		//SDL_Delay(0);
		returnValue = Frame();
		done = (returnValue != 0);

		//Frames per second calculator
		FPS += 1.0f;
		world->SetFrameNumber(world->GetFrameNumber()+1);
		if (SDL_GetTicks()-ticks > 1000)
		{
			FPS /= ((float)SDL_GetTicks()-ticks)/1000.0f;
			ticks = SDL_GetTicks();
			//printf("FPS: %f\n", FPS);
			guiManager->SetFPS(FPS);
			FPS = 0.0f;
			world->SetFrameNumber(0);
		}
	}

	rt_end = true;
	//SDL_WaitThread(thread, NULL);

	return returnValue;
}

int Game::Frame()
{
	int returnValue = 0;

	input->ResetSingleFrameInput();
	HandleInput();

	OrbitCamera();

	_los->ClearLineOfSight();
	RendererSingleton->AddLOS(_los);
	//RendererSingleton->AddTerrain(terrain);
	RendererSingleton->AddPhageTerrain(_phageTerrain);
	RendererSingleton->CalculateScene();

	TexturerSingleton->RunTextureWork();

	//This may need to be added into a separate class
	_rsGUITextureCursor.SetTexture(_textureCursor.id);
	int mX, mY;
	SDL_GetMouseState(&mX, &mY);
	_rsGUITextureCursor.SetPoints(mX, mY, mX+32, mY+32, 0.999f);
	RendererSingleton->AddGUITexture(_rsGUITextureCursor);

	//_bloom->Clear();
	//_bloom->RenderTest(world);

	//_shadowMap->Clear();
	//_shadowMap->RenderTest(world);

	//camera->GetShader()->UseProgram(SHADER_DEFAULT);

	mouseOverGUI = guiManager->ProcessMouse(world) == 1;

	_mouseUsedByActionManager = false;
	_mouseUsedByActionManager |= actionManager->ProcessInput(world) > 0;
	_mouseUsedByActionManager |= actionManager->Process(world, factionList) > 0;

	for (int i = 0;i < _gameSpeed;i ++)
	{
		entityManager->PreProcessEntities(world);
		entityManager->ProcessEntities(world);
		entityManager->PostProcessEntities(world);
		entityManager->HandleEntitySelection(world);

		_projectileSystem->ProcessProjectiles(world);
	}

	_projectileSystem->DrawProjectiles(world);

	if (!mouseOverGUI && input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
	{
		entityManager->SelectMouseOver(world);
	}
	if (mouseMotion && input->GetMouseInput(INPUT_MOUSE_RELEASE, INPUT_MOUSE_LEFTBUTTON))
	{
		entityManager->SelectMouseOver(world);
	}

	entityManager->Draw(world);

	//_squadronManager->Draw(world);

	//_particleSystem->Draw(world, rotate);

	actionManager->Draw(world, mouseOverGUI);

	//<PhCam>//guiManager->Draw(world, factionList, RendererSingleton->GetCamera(), objectManager->GetObjectSelectionArray(), mouseButtonLeftDown);
	guiManager->Draw(world, mouseButtonLeftDown);

	RendererSingleton->Render();

	//input->ResetSingleFrameInput();
	//HandleInput();

	if (!mouseOverGUI && input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
	{
		mouseButtonLeftDown = true;
		mouse1 = input->GetMousePosition();
		guiManager->SetUnitSelectionBoxPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}
	if (!mouseOverGUI && input->GetMouseInput(INPUT_MOUSE_RELEASE, INPUT_MOUSE_LEFTBUTTON))
	{
		mouseButtonLeftDown = false;
		mouseMotion = false;
	}
	if (input->GetMouseMotion())
	{
		if (mouseButtonLeftDown)
		{
			mouse2 = input->mousePosition;
			if (abs(mouse2.x-mouse1.x) > 10.0f || abs(mouse2.y-mouse1.y) > 10.0f)
				mouseMotion = true;
			guiManager->SetUnitSelectionBoxPoints(glm::vec4(mouse1, mouse2));
			entityManager->SetOrthoSquare(world, &glm::vec4(mouse1, mouse2));
		}
		RendererSingleton->SetMousePosition(input->mousePosition.x, input->mousePosition.y);
	}
	if (input->GetMouseWheel().y != 0)
	{
		float wheelVert = (float)input->GetMouseWheel().y;
		glm::vec3 vec = glm::vec3(0.0f, -0.7071f*wheelVert*10.0f, -0.7071f*wheelVert*10.0f);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));
		if (wheelVert < 0 || RendererSingleton->GetCameraPosition().y > 15.0f)
			RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
	}
	if (input->GetWindowSizeChange())
	{
		RendererSingleton->SetResolution(input->GetWindowSize().x, input->GetWindowSize().y);
		guiManager->PreRenderGUI(glm::vec2(RendererSingleton->GetScreenWidth(), RendererSingleton->GetScreenHeight()));
	}
	if (input->GetQuit())
	{
		returnValue = -1;
	}

	//_entityMap->ClearCheckedByMap();

	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'a'))
	{
		glm::vec3 vec = glm::vec3(-0.3f, 0.0f, 0.0f);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));
		//camera->moveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'd'))
	{
		glm::vec3 vec = glm::vec3(0.3f, 0.0f, 0.0f);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));
		//camera->moveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'w'))
	{
		glm::vec3 vec = glm::vec3(0.0f, 0.0f, -0.3f);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));
		//camera->moveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 's'))
	{
		glm::vec3 vec = glm::vec3(0.0f, 0.0f, 0.3f);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));
		//camera->moveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'q'))
	{
		float pitch = RendererSingleton->GetCameraPitch();
		float yZoom = glm::sin(glm::radians(pitch));
		float zZoom = glm::cos(glm::radians(pitch));
		glm::vec3 vec = glm::vec3(0.0f, yZoom, zZoom);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));

		//RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(0.0f, 1.0f, 0.0f);
		RendererSingleton->SetCameraPitch(RendererSingleton->GetCameraY() * 2.0f + 10.0f);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'e'))
	{
		float pitch = RendererSingleton->GetCameraPitch();
		float yZoom = glm::sin(glm::radians(pitch));
		float zZoom = glm::cos(glm::radians(pitch));
		glm::vec3 vec = glm::vec3(0.0f, -yZoom, -zZoom);
		vec = glm::rotate(vec, glm::radians(rotate), glm::vec3(0.0f, -1.0f, 0.0f));

		//RendererSingleton->MoveCameraPosition(vec.x, vec.y, vec.z);
		RendererSingleton->MoveCameraPosition(0.0f, -1.0f, 0.0f);
		RendererSingleton->SetCameraPitch(RendererSingleton->GetCameraY() * 2.0f + 10.0f);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'j'))
	{
		rotate -= 1.0f;
		RendererSingleton->SetCameraRotation(rotate);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'l'))
	{
		RendererSingleton->SetCameraRotation(rotate);
		rotate += 1.0f;
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'b'))
		boundingBoxOn = !boundingBoxOn;
	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'p'))
		terrainPaintOn = !terrainPaintOn;
	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'o'))
		terrainMapOn = !terrainMapOn;
	/*if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, '['))
		if (terrainPainter->GetLength() > 1)
			terrainPainter->SetLength(terrainPainter->GetLength()-1);
	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, ']'))
		terrainPainter->SetLength(terrainPainter->GetLength()+1);*/

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'h'))
		RendererSingleton->ToggleBumpMapping();

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, SDLK_KP_PLUS))
	{
		_gameSpeed ++;
		std::stringstream ss (std::stringstream::in | std::stringstream::out);
		ss << "Game Speed Changed: " << _gameSpeed;
		RendererSingleton->DebugConsole(ss.str());

		RendererSingleton->SetGameSpeed(_gameSpeed);
	}

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, SDLK_KP_MINUS) && _gameSpeed > 1)
	{
		_gameSpeed --;
		std::stringstream ss (std::stringstream::in | std::stringstream::out);
		ss << "Game Speed Changed: " << _gameSpeed;
		RendererSingleton->DebugConsole(ss.str());

		RendererSingleton->SetGameSpeed(_gameSpeed);
	}

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, '9'))
	{
		if (terrainPaintValue > 0) terrainPaintValue --;
		DebugOutput("TerrainPaintValue:\n", terrainPaintValue);
	}
	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, '0'))
	{
		if (terrainPaintValue < 10) terrainPaintValue ++;
		DebugOutput("TerrainPaintValue:\n", terrainPaintValue);
	}

	if (input->GetMouseInput(INPUT_MOUSE_DOWN, INPUT_MOUSE_LEFTBUTTON))
	{
		if (terrainPaintOn)
			_phageTerrain->PaintPosition((int)_phageTerrain->GetMouseOnTerrain().x, (int)_phageTerrain->GetMouseOnTerrain().z, terrainPaintValue);
	}

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'b'))
		RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_FLAG_RENDER_BOUNDING_BOXES);

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'f'))
		RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_FLAG_RENDER_WIREFRAME);

	if (input->GetKeyboardInput(INPUT_KEYBOARD_PRESSED, 'x'))
		RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_FLAG_RENDER_TERRAIN_MAP);
	if (RendererSingleton->GetDebugFlag(Renderer::DEBUG_FLAG_RENDER_TERRAIN_MAP))
	{
		world->GetPathFinder()->DebugDrawObstacleMap(world);
		world->GetPathFinder()->DebugDrawPathMap(world);
	}

	//Debug On-screen Menu
	if (input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
	{
		glm::vec2 mPos = input->GetMousePosition();
		if (mPos.x > RendererSingleton->GetScreenWidth()-180 && mPos.y > 330 && mPos.y < 340)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_SSAO);
		if (mPos.x > RendererSingleton->GetScreenWidth() - 180 && mPos.y > 340 && mPos.y < 350)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_BLUR_SSAO);
		if (mPos.x > RendererSingleton->GetScreenWidth() - 180 && mPos.y > 350 && mPos.y < 360)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_OUTLINE);
		if (mPos.x > RendererSingleton->GetScreenWidth() - 180 && mPos.y > 360 && mPos.y < 370)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_FXAA);
		if (mPos.x > RendererSingleton->GetScreenWidth() - 180 && mPos.y > 370 && mPos.y < 380)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_SHARPEN);
		if (mPos.x > RendererSingleton->GetScreenWidth() - 180 && mPos.y > 380 && mPos.y < 390)
			RendererSingleton->ToggleDebugFlag(Renderer::DEBUG_SHADOW);
	}

	return returnValue;
}

int Game::HandleInput()
{
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				input->mouseDown[sdlEvent.button.button-1] = true;
				if (!input->mousePressedEvent[sdlEvent.button.button-1])
					input->mousePressed[sdlEvent.button.button-1] = true;
				input->mousePressedEvent[sdlEvent.button.button-1] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				input->mouseDown[sdlEvent.button.button-1] = false;
				input->mouseRelease[sdlEvent.button.button-1] = true;
				input->mousePressedEvent[sdlEvent.button.button-1] = false;
				break;
			case SDL_MOUSEMOTION:
				input->mouseMotion = true;
				input->mousePosition.x = (float)sdlEvent.motion.x;
				input->mousePosition.y = (float)sdlEvent.motion.y;
				break;
			case SDL_MOUSEWHEEL:
				input->mouseWheel.x = sdlEvent.wheel.x;
				input->mouseWheel.y = sdlEvent.wheel.y;
				break;
			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						input->windowSizeChanged = true;
						input->windowSize.x = sdlEvent.window.data1;
						input->windowSize.y = sdlEvent.window.data2;
						break;
				}
				break;
			case SDL_KEYDOWN:
				input->keyboardDown[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				if (!input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)])
					input->keyboardPressed[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				break;
			case SDL_KEYUP:
				input->keyboardDown[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = false;
				input->keyboardRelease[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = false;
				break;
			case SDL_QUIT:
				input->quit = true;
				break;
		}
	}

	return 0;
}

int Game::OrbitCamera()
{
	//Orbit
	if (input->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_MIDDLEBUTTON))
	{
		orbitAmount = RendererSingleton->GetMouse().x;
		orbitAmountY = RendererSingleton->GetMouse().y;
		orbitMagnitude = glm::tan(glm::radians(35.0f))*RendererSingleton->GetCameraPosition().y;
		orbitX = RendererSingleton->GetCameraPosition().x + orbitMagnitude*glm::sin(glm::radians(rotate));
		orbitY = RendererSingleton->GetCameraPosition().z - orbitMagnitude*glm::cos(glm::radians(rotate));
		orbitMagnitude = glm::distance(glm::vec2(RendererSingleton->GetCameraPosition().x, RendererSingleton->GetCameraPosition().z), glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z));
		glm::vec3 ang;
		ang.x = RendererSingleton->GetCameraPosition().x - RendererSingleton->GetMouseTerrain().x;
		ang.z = RendererSingleton->GetCameraPosition().z - RendererSingleton->GetMouseTerrain().z;
		orbitBaseRotation = (-glm::atan(ang.x / ang.z))-glm::radians(rotate);
		if (ang.z < 0.0f) orbitBaseRotation += glm::radians(180.0f);
		orbitX = RendererSingleton->GetMouseTerrain().x;
		orbitY = RendererSingleton->GetMouseTerrain().z;
	}
	else if (input->GetMouseInput(INPUT_MOUSE_DOWN, INPUT_MOUSE_MIDDLEBUTTON))
	{
		float testAmount = (RendererSingleton->GetMouse().x - orbitAmount)/2.0f;
		SDL_WarpMouseInWindow(window, (int)orbitAmount, (int)orbitAmountY);
		//orbitAmount = input->GetMousePosition().x;

		rotate += testAmount;
		RendererSingleton->SetCameraPosition(orbitX - orbitMagnitude*glm::sin(glm::radians(rotate)+orbitBaseRotation), RendererSingleton->GetCameraPosition().y, orbitY + orbitMagnitude*glm::cos(glm::radians(rotate)+orbitBaseRotation));
		RendererSingleton->SetCameraPosition(orbitX - orbitMagnitude*glm::sin(glm::radians(rotate)+orbitBaseRotation), RendererSingleton->GetCameraPosition().y, orbitY + orbitMagnitude*glm::cos(glm::radians(rotate)+orbitBaseRotation));
		RendererSingleton->SetCameraRotation(rotate);
		DebugOutput("Base Rotation: %f\n", orbitBaseRotation);
	}

	return 0;
}
