#ifndef _ENTITY_BASE_H
#define _ENTITY_BASE_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityXML;
class Entity;

class Model;

class EntityBase
{
public:
	static const int TYPE_INVALID = -1;
	static const int TYPE_UNIT = 0;
	static const int TYPE_BUILDING = 1;
	static const int TYPE_OBJECT = 2;

	static const int HITBOX_MAX_COUNT = 3;
public:
	EntityBase();
	~EntityBase();

	int Init();

	Entity* CreateNewInstance(float x, float y, float rotation);
	int DeleteInstance(Entity* entity);

	int SetEntityXML(EntityXML* eXML);
	EntityXML* GetEntityXML();

	int SetModel(Model* model);
	Model* GetModel();

	//Accessors for XML tags
	std::string GetEntityName();
private:
	//This class is a friend to the EntityBaseCreator so that the variables can be set initially
	//without the class having setter functions
	friend class EntityXMLBaseCreator;

	//The index of the faction that this class belongs to
	int _factionIndex;

	int _entityTypeFlag;

	EntityXML* _entityXML;
	Model* _model;
};

#endif
