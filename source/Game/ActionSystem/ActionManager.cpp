#include "ActionManager.h"

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
#include "Game/Entity/EntityList.h"
#include "Game/EntityManager/EntityMap.h"

ActionManager::ActionManager()
{
	actionManagerInterface = NULL;
	objectInstanceCreator = NULL;
	currentState = ACTION_MANAGER_STATE_DEFAULT;

	buildStateEntityBase = NULL;
}

ActionManager::~ActionManager()
{
	delete actionManagerInterface;
	delete objectInstanceCreator;
}

int ActionManager::Init()
{
	actionManagerInterface = new ActionManagerInterface();
	objectInstanceCreator = new ObjectInstanceCreator();

	return 0;
}

int ActionManager::ProcessInput(World* world)
{
	if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_RIGHTBUTTON))
	{
		//Test for move to action
		//if (world->GetObjectSelector()->GetMouseOverObjects()->size() == 1)
		if (world->GetEntityMouseOverList()->Size() == 1)
		{
			//for (int i = 0;i < world->GetObjectSelectionArray()->Size();i ++)
			for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
			{
				//ObjectEventQueue* objectEventQueue = world->GetObjectSelectionArray()->Get(i)->getObjectEventQueue();
				EntityEventQueue* entityEventQueue = world->GetEntitySelectionList()->At(i)->GetEventQueue();
				if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
				{
					//world->GetObjectSelectionArray()->Get(i)->getObjectEventQueue()->ClearQueue();
					entityEventQueue->ClearAndDelete();
				}
				//if (world->GetObjectSelectionArray()->Get(i)->GetObjectClass()->getMovementSpeed() > 0.0f)
				if (world->GetEntitySelectionList()->At(i)->GetEntityBase()->MovementSpeed() > 0.0f)
				{
					//ObjectEvent* eventToEnqueue = new ObjectEvent();
					EntityEvent* eventToEnqueue = new EntityEvent();
					//eventToEnqueue->SetEventType(OBJECT_EVENT_MOVE_TO_INSTANCE);
					eventToEnqueue->eventType = EntityEvent::GATHER_FROM_INSTANCE;
					eventToEnqueue->owner = world->GetEntitySelectionList()->At(i);
					//eventToEnqueue->objectInstance = world->GetObjectSelector()->GetMouseOverObjects()->back();
					eventToEnqueue->entity = world->GetEntityMouseOverList()->Back();
					//objectEventQueue->Enqueue(eventToEnqueue);
					entityEventQueue->Enqueue(eventToEnqueue);
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
					eventToEnqueue->moveToPosition = glm::vec2(world->GetCamera()->getMouseTerrain().x, world->GetCamera()->getMouseTerrain().z);
					entityEventQueue->Enqueue(eventToEnqueue);
				}
			}
			else
			{
				//Create a new squadron for this action
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
						eventToEnqueue->moveToPosition = glm::vec2(world->GetCamera()->getMouseTerrain().x, world->GetCamera()->getMouseTerrain().z);
						eventToEnqueue->squadron = squadron;
						entityEventQueue->Enqueue(eventToEnqueue);
						squadron->AddEntity(entity);
					}
				}

				//Set the endpoint for the squadron
				squadron->SetEndpoint(world->GetCamera()->getMouseTerrain().x, world->GetCamera()->getMouseTerrain().z);

				//Perform the squadron creation
				squadron->CreateSquadron(world);
			}
		}
	}

	return 0;
}

int ActionManager::Process(World* world, FactionList* factionList)
{
	//Check for any pending actions
	Action* pendingAction = actionManagerInterface->actionFromActionPanel;
	if (pendingAction)
	{
		//Process action
		if (pendingAction->getActionType() == ACTION_TYPE_BUILD)
		{
			currentState = ACTION_MANAGER_STATE_BUILD;
			buildStateEntityBase = world->GetFactionBaseList()->At("Inferno Horde")->GetEntityBaseList()->At(pendingAction->getbuildObjectName());
			buildStateRotation = 0.0f;
		}
		else if (pendingAction->getActionType() == ACTION_TYPE_TRAIN)
		{
			if (world->GetEntitySelectionList()->Size() == 1)
			{
				EntityProcessEventQueue* entityProcessEventQueue = world->GetEntitySelectionList()->At(0)->GetProcessEventQueue();

				EntityProcessEvent* processEventToEnqueue = new EntityProcessEvent();
				processEventToEnqueue->eventType = EntityProcessEvent::TRAIN_ENTITY;
				processEventToEnqueue->owner = world->GetEntitySelectionList()->At(0);
				processEventToEnqueue->entityBase = world->GetFactionBaseList()->At("Inferno Horde")->GetEntityBaseList()->At(pendingAction->gettrainObjectName());
				processEventToEnqueue->trainPoints = processEventToEnqueue->entityBase->GetEntityXML()->trainpoints;
				entityProcessEventQueue->Enqueue(processEventToEnqueue);

				printf("Added train event for %s in %s\n", pendingAction->gettrainObjectName().c_str(), processEventToEnqueue->owner->GetEntityBase()->EntityName().c_str());
			}
		}

		//Reset the action panel action
		actionManagerInterface->actionFromActionPanel = NULL;
	}

	if (!world->IsMouseOverGUI())
	{
		if (currentState == ACTION_MANAGER_STATE_BUILD)
		{
			if (world->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
			{
				Entity* buildEntity;
				buildEntity = buildStateEntityBase->CreateNewInstance(world->GetCamera()->getMouseTerrain().x, world->GetCamera()->getMouseTerrain().z, buildStateRotation);
				world->GetPlayerList()->GetAt(0)->GetEntityList()->Add(buildEntity);
				world->GetEntityList()->Add(buildEntity);
				world->GetEntityMap()->Add(buildEntity);
				for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
				{
					Entity* entity = world->GetEntitySelectionList()->At(i);
					EntityEventQueue* entityEventQueue = entity->GetEventQueue();
					if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
					{
						entityEventQueue->ClearAndDelete();
					}
					EntityEvent* eventToEnqueue = new EntityEvent();
					eventToEnqueue->eventType = EntityEvent::BUILD_INSTANCE;
					eventToEnqueue->owner = entity;
					eventToEnqueue->entity = buildEntity;
					entityEventQueue->Enqueue(eventToEnqueue);
				}
				currentState = ACTION_MANAGER_STATE_DEFAULT;
				buildStateEntityBase = NULL;
			}
		}
		else if (currentState == ACTION_MANAGER_STATE_DEFAULT)
		{

		}
	}

	return 0;
}

int ActionManager::Draw(World* world, bool mouseOverGUI)
{
	Input* input = world->GetInput();
	Camera* camera = world->GetCamera();

	if (!mouseOverGUI)
	{
		if (currentState == ACTION_MANAGER_STATE_BUILD)
		{
			//Check for rotation
			if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 'y'))
				buildStateRotation += 5.0f;
			if (input->GetKeyboardInput(INPUT_KEYBOARD_DOWN, 't'))
				buildStateRotation -= 5.0f;

			//Draw object
			//glEnable(GL_STENCIL_TEST);
			//glStencilFunc(GL_ALWAYS, 254, 0xFF);
			//glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

			camera->GetShader()->UseProgram(SHADER_DEFAULT);
			camera->GetMVPMatrix()->Push(MODEL);
			camera->GetShader()->SetLighting(0);
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 0.5f);
			camera->GetMVPMatrix()->translate(MODEL, camera->getMouseTerrain().x, camera->getMouseTerrain().y, camera->getMouseTerrain().z);
			camera->GetMVPMatrix()->rotate(MODEL, buildStateRotation, 0.0f, 1.0f, 0.0f);
			camera->UpdateMatrix(SHADER_DEFAULT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0);

			buildStateEntityBase->Model()->Draw(world, camera, 0, 0.0f);

			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
			camera->GetShader()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			camera->GetShader()->SetLighting(1);
			camera->GetMVPMatrix()->Pop(MODEL);
			//glDisable(GL_STENCIL_TEST);
		}
	}

	return 0;
}
