#include "EntityBaseCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityModelXMLCreator.h"
#include "Game/EntitySystem/EntityBase.h"
#include "Parser/EntityXML.h"

#include "Graphics/Model/Model.h"

EntityBaseCreator::EntityBaseCreator()
{
	_entityModelXMLCreator = NULL;
}

EntityBaseCreator::~EntityBaseCreator()
{
	delete _entityModelXMLCreator;
}

int EntityBaseCreator::Init()
{
	_entityModelXMLCreator = new EntityModelXMLCreator();
	_entityModelXMLCreator->Init(true);

	return 0;
}

EntityBase* EntityBaseCreator::CreateEntityBase(EntityXML* entityXML)
{
	EntityBase* entityBase = new EntityBase();

	entityBase->SetEntityXML(entityXML);
	int errorOut;
	Model* model = _entityModelXMLCreator->CreateFromFile(entityXML->modelFile, &errorOut);
	if (errorOut == EntityModelXMLCreator::NONE)
		entityBase->SetModel(model);

	return entityBase;
}
