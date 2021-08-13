#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntitySystem;

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	//Initializes the entity manager
	int Init();

	//Renders all world entities
	int Draw(EntitySystem* entitySystem);

	//PreProcesses events for all entities.
	//For movement, this calculates initial vectors to path nodes.
	int PreProcessEntities(EntitySystem* entitySystem);

	//Process events for all entities. This is the majority of events that will be handled.
	//For movement, this adjusts vectors for collision avoidance and moves them.
	int ProcessEntities(EntitySystem* entitySystem);

	//PostProcesses events for all entities. This is where cleanup happens to ensure entities didn't do something they weren't suppose to.
	//For movement, this resolves any collisions that weren't suppose to happen.
	int PostProcessEntities(EntitySystem* entitySystem);

	int HandleEntitySelection(EntitySystem* entitySystem);
	int SetOrthoSquare(EntitySystem* entitySystem, glm::vec4* points);
	int SelectMouseOver(EntitySystem* entitySystem);
private:
};

#endif
