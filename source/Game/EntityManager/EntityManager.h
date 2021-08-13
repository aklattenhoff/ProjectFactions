#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include "Game/World.h"

#include "Game/EntityManager/EntityRenderer.h"
#include "Game/EntityManager/EntitySelectionHandler.h"

#include "Game/EntityManager/DebugEntityRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityAnimationHandler;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	//Initializes the entity manager
	int Init();

	//Renders all world entities
	int Draw(World* world);

	//PreProcesses events for all entities.
	//For movement, this calculates initial vectors to path nodes.
	int PreProcessEntities(World* world);

	//Process events for all entities. This is the majority of events that will be handled.
	//For movement, this adjusts vectors for collision avoidance and moves them.
	int ProcessEntities(World* world);

	//PostProcesses events for all entities. This is where cleanup happens to ensure entities didn't do something they weren't suppose to.
	//For movement, this resolves any collisions that weren't suppose to happen.
	int PostProcessEntities(World* world);

	int HandleEntitySelection(World* world);
	int SetOrthoSquare(World* world, glm::vec4* points);
	int SelectMouseOver(World* world);
private:
	EntityRenderer* _entityRenderer;
	EntitySelectionHandler* _entitySelectionHandler;

	DebugEntityRenderer* _debugEntityRenderer;
	bool _debugDrawFlag;

	EntityAnimationHandler* _entityAnimationHandler;
};

#endif
