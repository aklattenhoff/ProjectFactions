#include "ActionManager.h"


#include "Game/ActionManager/ActionProgressQueue.h"

#include "Game/EntityManager/EntityMouseOverList.h"
#include "Game/EntityManager/EntitySelectionList.h"
#include "Game/Entity/EntityEventQueue.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityProcessEventQueue.h"
#include "Game/Entity/EntityProcessEvent.h"
#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"

#include "Game/SquadronManager/SquadronManager.h"
#include "Game/SquadronManager/Squadron.h"

#include "Game/Faction/FactionBaseList.h"

#include "Game/Player/PlayerList.h"
#include "Game/Player/Player.h"
#include "Game/Entity/EntityList.h"
#include "Game/EntityManager/EntityMap.h"
#include "Game/PathFinder/PathFinder.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/Prop.h"
#include "Renderer/RS/Entity.h"
#include "Renderer/RS/PhageEntity.h"

ActionManager::ActionManager()
{
	actionManagerInterface = NULL;

	currentState = ACTION_MANAGER_STATE_DEFAULT;
	buildStateEntityBase = NULL;

	_buildStatePhageEntity = NULL;
}

ActionManager::~ActionManager()
{
	delete actionManagerInterface;

	delete _buildStatePhageEntity;
}

int ActionManager::Init()
{
	actionManagerInterface = new ActionManagerInterface();

	_buildStatePhageModel = new Phage::PhageModel();
	_buildStatePhageModel->Init();

	_buildStatePhageEntity = new Phage::PhageEntity();
	_buildStatePhageEntity->Init();
	_buildStatePhageEntity->SetPhageModel(_buildStatePhageModel);

	_buildStatePhageWallConnectorModel = new Phage::PhageModel();
	_buildStatePhageWallConnectorModel->Init();

	_buildStatePhageWallConnectorEntity = new Phage::PhageEntity();
	_buildStatePhageWallConnectorEntity->Init();
	_buildStatePhageWallConnectorEntity->SetPhageModel(_buildStatePhageWallConnectorModel);

	return 0;
}

int ActionManager::ProcessInput(World* world)
{
	int returnValue = 0;

	if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_RIGHTBUTTON) && currentState == ACTION_MANAGER_STATE_DEFAULT)
	{
		//Are we moused over something?
		if (world->GetEntityMouseOverList()->Size() == 1)
		{
			//First we need the average of the "core" group if there is one
			glm::vec2 coreTotal = glm::vec2(0.0f, 0.0f);
			float coreNum = 0.0f;
			glm::vec2 coreAvg = glm::vec2(0.0f, 0.0f);
			for (int i = 0; i < world->GetEntitySelectionList()->Size(); i++)
			{
				glm::vec2 entPos = world->GetEntitySelectionList()->At(i)->Pos2D();

				if ((i == 0) || (i > 0 && glm::distance(entPos, coreAvg) < 5.0f))
				{
					coreTotal += entPos;
					coreNum += 1.0f;
					coreAvg = coreTotal / coreNum;
				}
			}

			for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
			{
				EntityEventQueue* entityEventQueue = world->GetEntitySelectionList()->At(i)->GetEventQueue();
				if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
				{
					entityEventQueue->ClearAndDelete();
				}
				if (world->GetEntitySelectionList()->At(i)->GetEntityBase()->MovementSpeed() > 0.0f)
				{
					EntityEvent* eventToEnqueue = new EntityEvent();
					int ourTeam = world->GetPlayerList()->GetGameOwner();
					int targetTeam = world->GetPlayerList()->GetAt(world->GetEntityMouseOverList()->Back()->GetOwningPlayer())->GetTeam();
					int entityTeam = world->GetPlayerList()->GetAt(world->GetEntitySelectionList()->At(i)->GetOwningPlayer())->GetTeam();
					EntityBase* mouseOverBase = world->GetEntityMouseOverList()->Back()->GetEntityBase();
					if (targetTeam == 0)	//This is Gaia's team
					{
						if (world->GetEntityMouseOverList()->Back()->GetEntityBase()->ContainsClass("Gatherable"))
							eventToEnqueue->eventType = EntityEvent::GATHER_FROM_INSTANCE;
						else if (mouseOverBase->ContainsClass("Huntable"))
							eventToEnqueue->eventType = EntityEvent::HUNT_INSTANCE;
					}
					else if (targetTeam == ourTeam)	//This is our team
					{
						if (world->GetEntityMouseOverList()->Back()->GetEntityBase()->ContainsClass("Gatherable"))
							eventToEnqueue->eventType = EntityEvent::GATHER_FROM_INSTANCE;
						else if (world->GetEntityMouseOverList()->Back()->GetEntityBase()->ContainsClass("Buildable"))
							eventToEnqueue->eventType = EntityEvent::BUILD_INSTANCE;
						else
							eventToEnqueue->eventType = EntityEvent::MOVE_TO_INSTANCE;
					}
					else	//This is an enemy team
					{
						eventToEnqueue->eventType = EntityEvent::ATTACK_INSTANCE;
					}
					eventToEnqueue->owner = world->GetEntitySelectionList()->At(i);
					eventToEnqueue->entity = world->GetEntityMouseOverList()->Back();
					eventToEnqueue->moveToPosition = glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);

					glm::vec2 entPos = world->GetEntitySelectionList()->At(i)->Pos2D();
					if (eventToEnqueue->eventType == EntityEvent::MOVE_TO_INSTANCE && glm::distance(coreAvg, entPos) < 5.0f)
						eventToEnqueue->moveToPosition += entPos - coreAvg;

					//For right now lets only add the position of the entity for moveToInstance
					//This fixes a offset movement
					//TODO: Look into changing this
					if (eventToEnqueue->eventType == EntityEvent::MOVE_TO_INSTANCE)
						eventToEnqueue->entity = NULL;

					if (glm::distance(eventToEnqueue->moveToPosition, entPos) > 0.5f)
						entityEventQueue->Enqueue(eventToEnqueue);

					returnValue = 1;
				}
			}
		}
		else
		{
			if (world->GetEntitySelectionList()->Size() == 1)
			{
				EntityEventQueue* entityEventQueue = world->GetEntitySelectionList()->At(0)->GetEventQueue();
				if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
				{
					entityEventQueue->ClearAndDelete();
				}
				if (world->GetEntitySelectionList()->At(0)->GetEntityBase()->MovementSpeed() > 0.0f)
				{
					EntityEvent* eventToEnqueue = new EntityEvent();
					eventToEnqueue->eventType = EntityEvent::MOVE_TO_POSITION;
					eventToEnqueue->owner = world->GetEntitySelectionList()->At(0);
					eventToEnqueue->moveToPosition = glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);
					entityEventQueue->Enqueue(eventToEnqueue);

					returnValue = 1;
				}
			}
			else
			{
				//First we need the average of the "core" group if there is one
				glm::vec2 coreTotal = glm::vec2(0.0f, 0.0f);
				float coreNum = 0.0f;
				glm::vec2 coreAvg = glm::vec2(0.0f, 0.0f);
				for (int i = 0; i < world->GetEntitySelectionList()->Size(); i++)
				{
					glm::vec2 entPos = world->GetEntitySelectionList()->At(i)->Pos2D();
					if ((i == 0) || (i > 0 && glm::distance(entPos, coreAvg) < 5.0f))
					{
						coreTotal += entPos;
						coreNum += 1.0f;
						coreAvg = coreTotal / coreNum;
					}
				}

				for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
				{
					EntityEventQueue* entityEventQueue = world->GetEntitySelectionList()->At(i)->GetEventQueue();
					if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
					{
						entityEventQueue->ClearAndDelete();
					}
					if (world->GetEntitySelectionList()->At(0)->GetEntityBase()->MovementSpeed() > 0.0f)
					{
						EntityEvent* eventToEnqueue = new EntityEvent();
						eventToEnqueue->eventType = EntityEvent::MOVE_TO_POSITION;
						eventToEnqueue->owner = world->GetEntitySelectionList()->At(i);
						eventToEnqueue->moveToPosition = glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);

						glm::vec2 entPos = world->GetEntitySelectionList()->At(i)->Pos2D();
						if (glm::distance(coreAvg, entPos) < 5.0f)
							eventToEnqueue->moveToPosition += entPos - coreAvg;

						if (glm::distance(eventToEnqueue->moveToPosition, entPos) > 0.5f)
							entityEventQueue->Enqueue(eventToEnqueue);

						returnValue = 1;
					}
				}
				/*//Create a new squadron for this action
				Squadron* squadron = world->GetSquadronManager()->CreateSquadron();

				//Add entities to squadron and push event to entities
				for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
				{
					Entity* entity = world->GetEntitySelectionList()->At(i);
					EntityEventQueue* entityEventQueue = entity->GetEventQueue();
					if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
					{
						entityEventQueue->ClearAndDelete();
					}
					if (entity->GetEntityBase()->MovementSpeed() > 0.0f)
					{
						EntityEvent* eventToEnqueue = new EntityEvent();
						eventToEnqueue->eventType = EntityEvent::MOVE_IN_SQUADRON_TO_POSITION;
						eventToEnqueue->owner = entity;
						eventToEnqueue->moveToPosition = glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);
						eventToEnqueue->squadron = squadron;
						entityEventQueue->Enqueue(eventToEnqueue);
						squadron->AddEntity(entity);

						returnValue = 1;
					}
				}

				//Set the endpoint for the squadron
				squadron->SetEndpoint(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);

				//Perform the squadron creation
				squadron->CreateSquadron(world);*/
			}
		}
	}

	return returnValue;
}

int ActionManager::Process(World* world, FactionList* factionList)
{
	int returnValue = 0;

	//Check for any pending actions
	Action* pendingAction = actionManagerInterface->actionFromActionPanel;
	Player* player = world->GetPlayerList()->GetAt(world->GetPlayerList()->GetGameOwner());
	if (pendingAction)
	{
		//Process action
		if (pendingAction->getActionType() == ACTION_TYPE_BUILD)
		{
			EntityBase* buildBase = world->GetEntityBaseList()->At(pendingAction->getbuildObjectName());
			std::vector<EntityXMLTag::Cost> costVec =  buildBase->GetEntityXML()->costVector;
			bool hasResources = true;
			for (int i = 0;i < (int)costVec.size();i ++)
			{
				if (player->GetResourceValue(costVec[i].type) < costVec[i].cost)
					hasResources = false;
			}
			if (hasResources)
			{
				currentState = ACTION_MANAGER_STATE_BUILD;
				buildStateEntityBase = world->GetEntityBaseList()->At(pendingAction->getbuildObjectName());
				buildStateRotation = 0.0f;
			}
			else
			{
				currentState = ACTION_MANAGER_STATE_DEFAULT;
				buildStateEntityBase = NULL;
			}
		}
		else if (pendingAction->getActionType() == ACTION_TYPE_BUILD_WALL)
		{
			DebugOutput("Entered build wall state\n");
			currentState = ACTION_MANAGER_STATE_BUILD_WALL;
			_wallSegmentVector.clear();
			_currentWallSegment = 0;
			//buildStateEntityBaseWallConnector = world->GetEntityBaseList()->At(pendingAction->getbuildWallConnectorObjectName());
			//buildStateEntityBaseWall2x1 = world->GetEntityBaseList()->At(pendingAction->getbuildWall2x1ObjectName());
			//buildStateEntityBaseWall4x1 = world->GetEntityBaseList()->At(pendingAction->getbuildWall4x1ObjectName());
			//buildStateEntityBaseWall6x1 = world->GetEntityBaseList()->At(pendingAction->getbuildWall6x1ObjectName());
			//buildStateRotation = 0.0f;
		}
		else if (pendingAction->getActionType() == ACTION_TYPE_TRAIN)
		{
			if (world->GetEntitySelectionList()->Size() == 1)
			{
				EntityProcessEventQueue* entityProcessEventQueue = world->GetEntitySelectionList()->At(0)->GetProcessEventQueue();

				EntityProcessEvent* processEventToEnqueue = new EntityProcessEvent();
				processEventToEnqueue->eventType = EntityProcessEvent::TRAIN_ENTITY;
				processEventToEnqueue->owner = world->GetEntitySelectionList()->At(0);
				processEventToEnqueue->entityBase = world->GetEntityBaseList()->At(pendingAction->gettrainObjectName());
				processEventToEnqueue->trainPoints = processEventToEnqueue->entityBase->GetEntityXML()->trainpoints;
				entityProcessEventQueue->Enqueue(processEventToEnqueue);

				//ActionProgressQueue
				world->GetActionProgressQueue()->AddActionProgress(processEventToEnqueue, pendingAction->getActionIconTexture());
				//End-ActionProgressQueue

				printf("Added train event for %s in %s\n", pendingAction->gettrainObjectName().c_str(), processEventToEnqueue->owner->GetEntityBase()->EntityName().c_str());
			}
		}
		else if (pendingAction->getActionType() == ACTION_TYPE_UPGRADE)
		{
			if (world->GetEntitySelectionList()->Size() == 1)
			{
				Entity* entityToDelete = world->GetEntitySelectionList()->At(0);
				world->GetPlayerList()->GetAt(0)->GetEntityList()->Remove(entityToDelete);
				world->GetEntityList()->Remove(entityToDelete);
				world->GetEntityMap()->Remove(entityToDelete);

				Entity* e = world->GetEntityBaseList()->At(pendingAction->getupgradeObjectName())->CreateNewInstance(entityToDelete->PosX(), entityToDelete->PosZ(), entityToDelete->RotY(), 0, world->GetPhageTerrain()->CalculateHeight(entityToDelete->PosX(), entityToDelete->PosZ()));
				world->GetPlayerList()->GetAt(0)->GetEntityList()->Add(e);
				world->GetEntityList()->Add(e);
				world->GetEntityMap()->Add(e);

				for (int i = 0;i < (int)entityToDelete->GetEntityBase()->BoundingBox2D()->Size();i ++)
					//world->GetTerrain()->getTerrainMap()->SetRectangle(entityToDelete->GetEntityBase()->BoundingBox2D()->Get(i),
								//entityToDelete->GetTranslationVector(), entityToDelete->RotY(), 0, entityToDelete->GetTerrainMapCellsVectorPointer());
					world->GetPathFinder()->GenerateObstacleSubsetGroup(entityToDelete->GetEntityBase()->BoundingBox2D()->Get(i),
								entityToDelete->GetTranslationVector(), entityToDelete->RotY(), 0, entityToDelete->GetObstacleSubsetGroupPointer());

				for (int i = 0;i < (int)e->GetEntityBase()->BoundingBox2D()->Size();i ++)
					//world->GetTerrain()->getTerrainMap()->SetRectangle(e->GetEntityBase()->BoundingBox2D()->Get(i),
								//e->GetTranslationVector(), e->RotY(), 1, e->GetTerrainMapCellsVectorPointer());
					world->GetPathFinder()->GenerateObstacleSubsetGroup(e->GetEntityBase()->BoundingBox2D()->Get(i),
								e->GetTranslationVector(), e->RotY(), 1, e->GetObstacleSubsetGroupPointer());

				//delete entityToDelete;
			}
		}

		//Reset the action panel action
		actionManagerInterface->actionFromActionPanel = NULL;
	}

	//Process current state
	if (!world->IsMouseOverGUI())
	{
		if (currentState == ACTION_MANAGER_STATE_BUILD)
		{
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
			{
				//Disable left click for rest of frame
				world->GetInput()->ConsumeMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON);

				std::vector<EntityXMLTag::Cost> costVec =  buildStateEntityBase->GetEntityXML()->costVector;
				bool hasResources = true;
				for (int i = 0;i < (int)costVec.size();i ++)
				{
					if (player->GetResourceValue(costVec[i].type) < costVec[i].cost)
						hasResources = false;
				}
				if (hasResources)
				{
					for (int i = 0;i < (int)costVec.size();i ++)
					{
						player->ModifyResource(Player::GetResourceIndex(costVec[i].type), -costVec[i].cost);
					}

					Entity* buildEntity;
					buildEntity = buildStateEntityBase->CreateNewInstance(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z, buildStateRotation, world->GetPlayerList()->GetGameOwner(), world->GetPhageTerrain()->CalculateHeight(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z));
					world->GetPlayerList()->GetAt(0)->GetEntityList()->Add(buildEntity);
					world->GetEntityList()->Add(buildEntity);
					world->GetEntityMap()->Add(buildEntity);
					//TerrainMapCellsVector terrainMapCellsVec;
					for (int i = 0;i < (int)buildEntity->GetEntityBase()->BoundingBox2D()->Size();i ++)
						//world->GetTerrain()->getTerrainMap()->SetRectangle(buildEntity->GetEntityBase()->BoundingBox2D()->Get(i), buildEntity->GetTranslationVector(), buildEntity->RotY(), 1, buildEntity->GetTerrainMapCellsVectorPointer());
						world->GetPathFinder()->GenerateObstacleSubsetGroup(buildEntity->GetEntityBase()->BoundingBox2D()->Get(i), buildEntity->GetTranslationVector(), buildEntity->RotY(), 1, buildEntity->GetObstacleSubsetGroupPointer());
					EntityXMLTag::TerrainDecalSet terrDec = buildStateEntityBase->GetEntityXML()->terrainDecalSet;
					for (unsigned int i = 0;i < terrDec.size();i ++)
					{
						glm::vec2 decPt1 = glm::rotate(glm::vec2(terrDec[i].pt1.x, terrDec[i].pt1.y), -glm::radians(buildStateRotation));
						glm::vec2 decPt2 = glm::rotate(glm::vec2(terrDec[i].pt2.x, terrDec[i].pt2.y), -glm::radians(buildStateRotation));
						glm::vec2 decPos = (decPt1+decPt2)/2.0f + glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);
						glm::vec2 decSize = glm::vec2(abs(terrDec[i].pt2.x-terrDec[i].pt1.x), terrDec[i].size.width);
						float decRot = -glm::degrees(atan2(decPt2.y-decPt1.y, decPt2.x-decPt1.x));// + buildStateRotation;
						world->GetPhageTerrain()->CreateAndAddTerrainDecal(terrDec[i].texture, decPos.x, decPos.y, decRot, decSize.x, decSize.y);
					}
					for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
					{
						Entity* entity = world->GetEntitySelectionList()->At(i);
						EntityEventQueue* entityEventQueue = entity->GetEventQueue();
						if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
						{
							entityEventQueue->ClearAndDelete();
						}
						if (entity->GetEntityBase()->MovementSpeed() > 0.0f)
						{
							EntityEvent* eventToEnqueue = new EntityEvent();
							eventToEnqueue->eventType = EntityEvent::BUILD_INSTANCE;
							eventToEnqueue->owner = entity;
							eventToEnqueue->entity = buildEntity;
							eventToEnqueue->moveToPosition = buildEntity->Pos2D();
							entityEventQueue->Enqueue(eventToEnqueue);
						}
					}
					if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
					{
						currentState = ACTION_MANAGER_STATE_DEFAULT;
						buildStateEntityBase = NULL;
					}

					returnValue = 1;
				}
			}
			else if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_RIGHTBUTTON))
			{
				currentState = ACTION_MANAGER_STATE_DEFAULT;
				buildStateEntityBase = NULL;

				returnValue = 1;
			}
		}
		else if (currentState == ACTION_MANAGER_STATE_BUILD_WALL)
		{
			glm::vec3 mouseTerrain = RendererSingleton->GetMouseTerrain();
			float towerRadius = 2.0f;
			_hoverTower = -1;
			for (unsigned int wallConnItr = 0; wallConnItr < _wallSegmentVector.size(); wallConnItr++)
			{
				float distanceTower = glm::distance(_wallSegmentVector[wallConnItr].pos, mouseTerrain);
				if (distanceTower < towerRadius)
				{
					_hoverTower = wallConnItr;
					towerRadius = distanceTower;
				}
			}
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
			{
				if (_hoverTower != -1)
				{
					if (_hoverTower != _currentWallSegment)
					{
						std::vector<int> hover = _wallSegmentVector[_hoverTower].conns;
						std::vector<int> current = _wallSegmentVector[_currentWallSegment].conns;
						if (std::find(hover.begin(), hover.end(), _currentWallSegment) == hover.end() && std::find(current.begin(), current.end(), _hoverTower) == current.end())
						{
							DebugOutput("Added new connection to segment\n");
							_wallSegmentVector[_hoverTower].conns.push_back(_currentWallSegment);
							_currentWallSegment = _hoverTower;
						}
					}
				}
				else
				{
					DebugOutput("Added new segment to wall\n");
					_wallSegmentVector.push_back(WallSegment());
					_wallSegmentVector.back().pos = RendererSingleton->GetMouseTerrain();
					_wallSegmentVector.back().conns.push_back(_currentWallSegment);
					_currentWallSegment = _wallSegmentVector.size() - 1;
				}
			}
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_RIGHTBUTTON))
			{
				_finishWallTimer = SDL_GetTicks();
			}
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_DOWN, INPUT_MOUSE_RIGHTBUTTON) && SDL_GetTicks() - _finishWallTimer > 1000)
			{
				currentState = ACTION_MANAGER_STATE_DEFAULT;
			}
			else if (world->GetInput()->GetMouseInput(INPUT_MOUSE_RELEASE, INPUT_MOUSE_RIGHTBUTTON))
			{
				if (_wallSegmentVector.size() > 0)
				{
					if (_wallSegmentVector[_currentWallSegment].conns.size() > 1)
					{
						int prevWallSegment = _wallSegmentVector[_currentWallSegment].conns.back();
						_wallSegmentVector[_currentWallSegment].conns.pop_back();
						_currentWallSegment = prevWallSegment;
					}
					else
					{
						_currentWallSegment = _wallSegmentVector[_currentWallSegment].conns.back();
						_wallSegmentVector.pop_back();
					}
				}
			}
		}
		else if (currentState == ACTION_MANAGER_STATE_BUILD_WALL_PHASE_2)
		{
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
			{
				glm::vec2 finalPos = glm::vec2(RendererSingleton->GetMouseTerrain().x, RendererSingleton->GetMouseTerrain().z);
				if (world->GetEntityMouseOverList()->Size() > 0 && world->GetEntityMouseOverList()->At(0)->GetEntityBase() == buildStateEntityBaseWallConnector)
				{
					FinishAtExistingConnector = true;
					finalPos.x = world->GetEntityMouseOverList()->At(0)->PosX();
					finalPos.y = world->GetEntityMouseOverList()->At(0)->PosZ();
				}
				else
					FinishAtExistingConnector = false;

				float direction = -atan2(finalPos.y-wallAnchorPoint.z, finalPos.x-wallAnchorPoint.x);
				float distance = glm::distance(glm::vec2(finalPos.x, finalPos.y), glm::vec2(wallAnchorPoint.x, wallAnchorPoint.z));
				buildStateRotation = glm::degrees(direction);
				glm::vec3 lastPos = wallAnchorPoint;
				glm::vec3 currentPos = wallAnchorPoint;

				if (startFromExistingConnector)
				{
				}
				else
				{
				CreateInstanceAndAddToQueue(world, buildStateEntityBaseWallConnector, glm::vec2(wallAnchorPoint.x, wallAnchorPoint.z), buildStateRotation, true);
				}

				bool wallDone = false;
				while (!wallDone)
				{
					if (distance > 8.0f)
					{
						currentPos.x += 7.0f*cos(direction);
						currentPos.z -= 7.0f*sin(direction);

						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWall6x1, glm::vec2((lastPos.x+currentPos.x)/2.0f, (lastPos.z+currentPos.z)/2.0f), buildStateRotation, false);

						lastPos = currentPos;
						distance -= 7.0f;
					}
					else if (distance > 7.0f)
					{
						currentPos.x += 5.0f*cos(direction);
						currentPos.z -= 5.0f*sin(direction);

						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWall4x1, glm::vec2((lastPos.x+currentPos.x)/2.0f, (lastPos.z+currentPos.z)/2.0f), buildStateRotation, false);

						lastPos = currentPos;
						distance -= 5.0f;
					}
					else if (distance > 5.0f)
					{
						currentPos.x = finalPos.x;
						currentPos.z = finalPos.y;

						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWall6x1, glm::vec2((lastPos.x+currentPos.x)/2.0f, (lastPos.z+currentPos.z)/2.0f), buildStateRotation, false);

						wallDone = true;
					}
					else if (distance > 3.0f)
					{
						currentPos.x = finalPos.x;
						currentPos.z = finalPos.y;

						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWall4x1, glm::vec2((lastPos.x+currentPos.x)/2.0f, (lastPos.z+currentPos.z)/2.0f), buildStateRotation, false);

						wallDone = true;
					}
					else
					{
						currentPos.x = finalPos.x;
						currentPos.z = finalPos.y;

						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWall2x1, glm::vec2((lastPos.x+currentPos.x)/2.0f, (lastPos.z+currentPos.z)/2.0f), buildStateRotation, false);

						wallDone = true;
					}
					if (wallDone && FinishAtExistingConnector)
					{
					}
					else
					{
						CreateInstanceAndAddToQueue(world, buildStateEntityBaseWallConnector, glm::vec2(currentPos.x, currentPos.z), buildStateRotation, false);
					}
				}
				currentState = ACTION_MANAGER_STATE_DEFAULT;
				buildStateEntityBaseWallConnector = NULL;
				buildStateEntityBaseWall2x1 = NULL;
				buildStateEntityBaseWall4x1 = NULL;
				buildStateEntityBaseWall6x1 = NULL;
			}
		}
		else if (currentState == ACTION_MANAGER_STATE_DEFAULT)
		{

		}
	}

	return returnValue;
}

int ActionManager::Draw(World* world, bool mouseOverGUI)
{
	Input* input = world->GetInput();
	//Camera* camera = RendererSingleton->GetCamera();
	glm::vec3 mouseTerrain = RendererSingleton->GetMouseTerrain();

	if (!mouseOverGUI)
	{
		if (currentState == ACTION_MANAGER_STATE_BUILD)
		{
			//Check for rotation
			if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'y'))
				buildStateRotation += 5.0f;
			if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 't'))
				buildStateRotation -= 5.0f;

			RS::Entity rsEntity;
			rsEntity.SetEntityBase(buildStateEntityBase);
			rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
			rsEntity.SetAnimation(0, 0.0f);
			rsEntity.SetModel(buildStateEntityBase->GetModel(), glm::vec3(mouseTerrain.x, mouseTerrain.y, mouseTerrain.z), buildStateRotation);
			RendererSingleton->AddEntity(rsEntity);

			RS::PhageEntity rsPhageEntity;
			_buildStatePhageModel->SetModel(buildStateEntityBase->GetModel());
			_buildStatePhageModel->SetOrientation(Phage::PhageModel::ORIENTATION_Z);
			_buildStatePhageModel->SetScale(buildStateEntityBase->GetModel()->GetScale());
			_buildStatePhageEntity->SetBaseRotate(-90.0f, 1.0f, 0.0f, 0.0f);
			_buildStatePhageEntity->SetScale(_buildStatePhageModel->GetScale(), _buildStatePhageModel->GetScale(), _buildStatePhageModel->GetScale());
			rsPhageEntity.SetModel(_buildStatePhageEntity, glm::vec3(mouseTerrain.x, mouseTerrain.y, mouseTerrain.z), buildStateRotation);
			rsPhageEntity.SetAnimation(0, 0.0f);
			rsPhageEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
			rsPhageEntity.SetEntityBase(buildStateEntityBase);
			RendererSingleton->AddPhageEntity(rsPhageEntity);
		}
		else if (currentState == ACTION_MANAGER_STATE_BUILD_WALL)
		{
			for (unsigned int wallSegItr = 0; wallSegItr < _wallSegmentVector.size(); wallSegItr++)
			{
				std::vector<int> connVector = _wallSegmentVector[wallSegItr].conns;
				for (unsigned int connsItr = 0; connsItr < connVector.size(); connsItr++)
				{
					RendererSingleton->AddLineDrawInfo(_wallSegmentVector[wallSegItr].pos, _wallSegmentVector[connVector[connsItr]].pos);
				}

				glm::vec3 wallSegPos = _wallSegmentVector[wallSegItr].pos;
				glm::vec3 p1 = glm::vec3(wallSegPos.x - 0.5f, wallSegPos.y, wallSegPos.z - 0.5f);
				glm::vec3 p2 = glm::vec3(wallSegPos.x + 0.5f, wallSegPos.y, wallSegPos.z - 0.5f);
				glm::vec3 p3 = glm::vec3(wallSegPos.x + 0.5f, wallSegPos.y, wallSegPos.z + 0.5f);
				glm::vec3 p4 = glm::vec3(wallSegPos.x - 0.5f, wallSegPos.y, wallSegPos.z + 0.5f);
				RendererSingleton->AddLineDrawInfo(p1, p2);
				RendererSingleton->AddLineDrawInfo(p2, p3);
				RendererSingleton->AddLineDrawInfo(p3, p4);
				RendererSingleton->AddLineDrawInfo(p4, p1);
			}

			//if (_buildWallConnectors.size() > 0)
			{
				//RendererSingleton->AddLineDrawInfo(_buildWallConnectors.back(), mouseTerrain);
			}

			if (_hoverTower != -1)
			{
				glm::vec3 wallSegPos = _wallSegmentVector[_hoverTower].pos;
				glm::vec3 p1 = glm::vec3(wallSegPos.x - 0.5f, wallSegPos.y, wallSegPos.z - 0.5f);
				glm::vec3 p2 = glm::vec3(wallSegPos.x + 0.5f, wallSegPos.y, wallSegPos.z - 0.5f);
				glm::vec3 p3 = glm::vec3(wallSegPos.x + 0.5f, wallSegPos.y, wallSegPos.z + 0.5f);
				glm::vec3 p4 = glm::vec3(wallSegPos.x - 0.5f, wallSegPos.y, wallSegPos.z + 0.5f);
				RendererSingleton->AddLineDrawInfo(p1, p2);
				RendererSingleton->AddLineDrawInfo(p2, p3);
				RendererSingleton->AddLineDrawInfo(p3, p4);
				RendererSingleton->AddLineDrawInfo(p4, p1);

				if (_wallSegmentVector.size() > 0)
					RendererSingleton->AddLineDrawInfo(_wallSegmentVector[_currentWallSegment].pos, wallSegPos);
			}
			else
			{
				glm::vec3 p1 = glm::vec3(mouseTerrain.x - 0.5f, mouseTerrain.y, mouseTerrain.z - 0.5f);
				glm::vec3 p2 = glm::vec3(mouseTerrain.x + 0.5f, mouseTerrain.y, mouseTerrain.z - 0.5f);
				glm::vec3 p3 = glm::vec3(mouseTerrain.x + 0.5f, mouseTerrain.y, mouseTerrain.z + 0.5f);
				glm::vec3 p4 = glm::vec3(mouseTerrain.x - 0.5f, mouseTerrain.y, mouseTerrain.z + 0.5f);
				RendererSingleton->AddLineDrawInfo(p1, p2);
				RendererSingleton->AddLineDrawInfo(p2, p3);
				RendererSingleton->AddLineDrawInfo(p3, p4);
				RendererSingleton->AddLineDrawInfo(p4, p1);

				if (_wallSegmentVector.size() > 0)
					RendererSingleton->AddLineDrawInfo(_wallSegmentVector[_currentWallSegment].pos, mouseTerrain);
			}
		}
		else if (currentState == ACTION_MANAGER_STATE_BUILD_WALL_PHASE_2)
		{
			glm::vec2 finalPos = glm::vec2(mouseTerrain.x, mouseTerrain.z);
			if (world->GetEntityMouseOverList()->Size() > 0 && world->GetEntityMouseOverList()->At(0)->GetEntityBase() == buildStateEntityBaseWallConnector)
			{
				FinishAtExistingConnector = true;
				finalPos.x = world->GetEntityMouseOverList()->At(0)->PosX();
				finalPos.y = world->GetEntityMouseOverList()->At(0)->PosZ();
			}
			else
				FinishAtExistingConnector = false;

			float direction = -atan2(finalPos.y-wallAnchorPoint.z, finalPos.x-wallAnchorPoint.x);
			float distance = glm::distance(glm::vec2(finalPos.x, finalPos.y), glm::vec2(wallAnchorPoint.x, wallAnchorPoint.z));
			buildStateRotation = glm::degrees(direction);
			glm::vec3 lastPos = wallAnchorPoint;
			glm::vec3 currentPos = wallAnchorPoint;

			if (startFromExistingConnector)
			{
			}
			else
			{
			RS::Entity rsEntity;
			rsEntity.SetEntityBase(buildStateEntityBaseWallConnector);
			rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
			rsEntity.SetAnimation(0, 0.0f);
			rsEntity.SetModel(buildStateEntityBaseWallConnector->GetModel(), glm::vec3(wallAnchorPoint.x, wallAnchorPoint.y, wallAnchorPoint.z), buildStateRotation);
			RendererSingleton->AddEntity(rsEntity);
			}

			bool wallDone = false;
			while (!wallDone)
			{
				if (distance > 8.0f)
				{
					currentPos.x += 7.0f*cos(direction);
					currentPos.z -= 7.0f*sin(direction);

					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWall6x1);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWall6x1->GetModel(),
						glm::vec3((lastPos.x+currentPos.x)/2.0f, mouseTerrain.y, (lastPos.z+currentPos.z)/2.0f), buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);

					lastPos = currentPos;
					distance -= 7.0f;
				}
				else if (distance > 7.0f)
				{
					currentPos.x += 5.0f*cos(direction);
					currentPos.z -= 5.0f*sin(direction);

					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWall4x1);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWall4x1->GetModel(),
						glm::vec3((lastPos.x+currentPos.x)/2.0f, mouseTerrain.y, (lastPos.z+currentPos.z)/2.0f), buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);

					lastPos = currentPos;
					distance -= 5.0f;
				}
				else if (distance > 5.0f)
				{
					currentPos.x = finalPos.x;
					currentPos.z = finalPos.y;

					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWall6x1);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWall6x1->GetModel(),
						glm::vec3((lastPos.x+currentPos.x)/2.0f, mouseTerrain.y, (lastPos.z+currentPos.z)/2.0f), buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);

					wallDone = true;
				}
				else if (distance > 3.0f)
				{
					currentPos.x = finalPos.x;
					currentPos.z = finalPos.y;

					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWall4x1);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWall4x1->GetModel(),
						glm::vec3((lastPos.x+currentPos.x)/2.0f, mouseTerrain.y, (lastPos.z+currentPos.z)/2.0f), buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);

					wallDone = true;
				}
				else
				{
					currentPos.x = finalPos.x;
					currentPos.z = finalPos.y;

					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWall2x1);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWall2x1->GetModel(),
						glm::vec3((lastPos.x+currentPos.x)/2.0f, mouseTerrain.y, (lastPos.z+currentPos.z)/2.0f), buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);

					wallDone = true;
				}
				if (wallDone && FinishAtExistingConnector)
				{
				}
				else
				{
					RS::Entity rsEntity;
					rsEntity.SetEntityBase(buildStateEntityBaseWallConnector);
					rsEntity.SetPlayer(world->GetPlayerList()->GetAt(0));
					rsEntity.SetAnimation(0, 0.0f);
					rsEntity.SetModel(buildStateEntityBaseWallConnector->GetModel(), currentPos, buildStateRotation);
					RendererSingleton->AddEntity(rsEntity);
				}
			}
		}
	}

	return 0;
}

int ActionManager::CreateInstanceAndAddToQueue(World* world, EntityBase* entBase, glm::vec2 pos, float rotation, bool clearQueue)
{
	Entity* buildEntity;
	buildEntity = entBase->CreateNewInstance(pos.x, pos.y, rotation, 0, world->GetPhageTerrain()->CalculateHeight(pos.x, pos.y));
	world->GetPlayerList()->GetAt(0)->GetEntityList()->Add(buildEntity);
	world->GetEntityList()->Add(buildEntity);
	world->GetEntityMap()->Add(buildEntity);
	//TerrainMapCellsVector terrainMapCellsVec;
	for (int i = 0;i < (int)buildEntity->GetEntityBase()->BoundingBox2D()->Size();i ++)
		//world->GetTerrain()->getTerrainMap()->SetRectangle(buildEntity->GetEntityBase()->BoundingBox2D()->Get(i), buildEntity->GetTranslationVector(), buildEntity->RotY(), 1, buildEntity->GetTerrainMapCellsVectorPointer());
		world->GetPathFinder()->GenerateObstacleSubsetGroup(buildEntity->GetEntityBase()->BoundingBox2D()->Get(i), buildEntity->GetTranslationVector(), buildEntity->RotY(), 1, buildEntity->GetObstacleSubsetGroupPointer());
	for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
	{
		Entity* entity = world->GetEntitySelectionList()->At(i);
		EntityEventQueue* entityEventQueue = entity->GetEventQueue();
		if (clearQueue && !world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
		{
			entityEventQueue->ClearAndDelete();
		}
		EntityEvent* eventToEnqueue = new EntityEvent();
		eventToEnqueue->eventType = EntityEvent::BUILD_INSTANCE;
		eventToEnqueue->owner = entity;
		eventToEnqueue->entity = buildEntity;
		entityEventQueue->Enqueue(eventToEnqueue);
	}

	return 0;
}
