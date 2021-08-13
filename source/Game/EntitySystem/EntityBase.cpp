#include "EntityBase.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/EntityXML.h"
#include "Game/EntitySystem/Entity.h"

#include "Graphics/Model/Model.h"

EntityBase::EntityBase()
{
	_entityXML = NULL;
	//selectionIndicator = NULL;
	//boundingBox2DSet = new BoundingBox2DSet();
	//boundingBox3DSet = new BoundingBox3DSet();
}

EntityBase::~EntityBase()
{
	delete _entityXML;
	//delete objectModelXMLParser;
	//delete objectModel;
	//delete selectionIndicator;
	//delete boundingBox2DSet;
	//delete boundingBox3DSet;
}

int EntityBase::Init()
{
	//selectionIndicator = new SelectionIndicator();
	//if (entityTypeInt != ENTITY_TYPE_BUILDING)
		//selectionIndicator->SetSelectionIndicatorType(SELECTION_INDICATOR_CIRCLE);
	//else
		//selectionIndicator->SetSelectionIndicatorType(SELECTION_INDICATOR_RECTANGLE);
	//selectionIndicator->SetSelectionIndicatorSize(1.0f, 1.0f);
	//selectionIndicator->Init();

	return 0;
}

Entity* EntityBase::CreateNewInstance(float x, float y, float rotation)
{
	Entity* entity = new Entity();

	entity->_entityBase = this;
	entity->_transformation.translation.x = x;
	entity->_transformation.translation.z = y;
	entity->_transformation.rotation.y = rotation;

	entity->Init();

	return entity;
}

int EntityBase::DeleteInstance(Entity* entity)
{
	delete entity;

	return 0;
}

int EntityBase::SetEntityXML(EntityXML* eXML)
{
	_entityXML = eXML;
	return 0;
}

EntityXML* EntityBase::GetEntityXML()
{
	return _entityXML;
}

int EntityBase::SetModel(Model* model)
{
	_model = model;
	return 0;
}

Model* EntityBase::GetModel()
{
	return _model;
}

std::string EntityBase::GetEntityName()
{
	return _entityXML->entityTag.name;
}
