#ifndef _ENTITY_SYSTEM_H
#define _ENTITY_SYSTEM_H

class EntityBaseListCreator;
class EntityManager;
class EntityRenderer;
class DebugEntityRenderer;

class Parser;
class EntityBase;
class Entity;
class EntityBaseList;
class EntityList;

class EntitySystem
{
public:
	EntitySystem();
	~EntitySystem();

	//Creates and initializes all dependencies of EntitySystem
	int CreateEntitySystem();

	//Parses the entity model xml and creates the entity bases based on the list
	//NOTE: This will also create the models and animation for each entity base
	int CreateEntityBaseList(Parser* parser);

	EntityManager* GetEntityManager();
	EntityRenderer* GetEntityRenderer();
	DebugEntityRenderer* GetDebugEntityRenderer();

	EntityBaseList* GetEntityBaseList();
	EntityList* GetEntityList();
private:
	EntityBaseListCreator* _entityBaseListCreator;
	EntityManager* _entityManager;
	EntityRenderer* _entityRenderer;
	DebugEntityRenderer* _debugEntityRenderer;

	EntityBaseList* _entityBaseList;
	EntityList* _entityList;
};
#endif
