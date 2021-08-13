#ifndef _ENTITY_BASE_H
#define _ENTITY_BASE_H

#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/BoundingBox3DSet.h"
#include "Game/Math/BoundingBox2DSet.h"

//#include "Game/ObjectModel/ObjectModelXMLParser.h"
//#include "Game/ObjectModel/ObjectModel.h"

//#include "Graphics/Model/Model.h"
#include "Phage/Backend/Model.h"
#include "Phage/Phage.h"

#include "Game/ActionManager/Action.h"

#include "Parser/EntityXML.h"
#include "Parser/EntityXMLTag.h"

#include "Game/Entity/Entity.h"

#include "Game/LOS/LOS.h"

const int ENTITY_TYPE_INVALID = -1;
const int ENTITY_TYPE_UNIT = 0;
const int ENTITY_TYPE_BUILDING = 1;
const int ENTITY_TYPE_OBJECT = 2;

const int ENTITY_BASE_HITBOX_MAX_COUNT = 3;

class EntityBase
{
public:
	EntityBase();
	~EntityBase();

	int Init();

	Entity* CreateNewInstance(float x, float y, float rotation, int owningPlayer, float height = 0.0f);
	int DeleteInstance(Entity* entity);

	int SetEntityXML(EntityXML* eXML);
	EntityXML* GetEntityXML() {return _entityXML;}

	std::string EntityName() {return _entityXML->entityTag.name;}
	std::string EntityType() {return _entityXML->entityTag.type;}
	int EntityTypeInt() {return entityTypeInt;}

	EntityXMLTag::MovementSpeed MovementSpeed() {return _entityXML->movementSpeed;}
	EntityXMLTag::MovementTurnRate MovementTurnRate() { return _entityXML->movementTurnRate; }
	EntityXMLTag::MovementCollisionRadius MovementCollisionRadius() { return _entityXML->movementCollisionRadius; }
	EntityXMLTag::MovementAwareRadius MovementAwareRadius() { return _entityXML->movementAwareRadius; }

	std::vector<EntityXMLTag::Action> ActionVector() {return _entityXML->actionVector;}

	//ObjectModel* GetObjectModel() {return objectModel;}

	int SetModel(Model* model);
	Model* GetModel();

	int SetUnbuiltModel(Model* model);
	Model* GetUnbuiltModel();

	int SetPhageModel(Phage::PhageModel* phageModel);
	Phage::PhageModel* GetPhageModel();

	int SetUnbuiltPhageModel(Phage::PhageModel* phageModel);
	Phage::PhageModel* GetUnbuiltPhageModel();

	BoundingBox3DSet* BoundingBox3D() {return boundingBox3DSet;}
	BoundingBox2DSet* BoundingBox2D() {return boundingBox2DSet;}
	BoundingBox2DSet* GatherArea() {return gatherArea;}

	int SetLOSPoints(LOS::LOSPoints* points);
	LOS::LOSPoints* GetLOSPointsPointer();

	bool ContainsClass(std::string c);
private:
	//This class is a friend to the EntityBaseCreator so that the variables can be set initially
	//without the class having setter functions
	friend class EntityBaseCreator;

	//The index of the faction that this class belongs to
	int factionIndex;

	int entityTypeInt;

	EntityXML* _entityXML;
	std::map<std::string, bool> _classMap;

	//Model loading
	//ObjectModelXMLParser* objectModelXMLParser;
	//ObjectModel* objectModel;

	Model* _model;
	Model* _unbuiltModel;

	Phage::PhageModel* _phageModel;
	Phage::PhageModel* _unbuiltPhageModel;

	//BoundingBox2DSet
	BoundingBox2DSet* boundingBox2DSet;
	//BoundingBox3DSet
	BoundingBox3DSet* boundingBox3DSet;
	//GatherArea
	BoundingBox2DSet* gatherArea;

	LOS::LOSPoints* _losPoints;
};

#endif
