#include "TempEntityBaseCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/TempEntityCreator/TempEntityModelXMLCreator.h"
#include "Game/Entity/EntityBase.h"
#include "Parser/EntityXML.h"

#include "Phage/Backend/Model.h"
#include "Phage/Phage.h"

#include "Game/ProjectileSystem/ProjectileBase.h"
#include "Parser/ProjectileXML.h"

TempEntityBaseCreator::TempEntityBaseCreator()
{
	_entityModelXMLCreator = NULL;
}

TempEntityBaseCreator::~TempEntityBaseCreator()
{
	delete _entityModelXMLCreator;
}

int TempEntityBaseCreator::Init()
{
	_entityModelXMLCreator = new TempEntityModelXMLCreator();
	_entityModelXMLCreator->Init(true);

	return 0;
}

EntityBase* TempEntityBaseCreator::CreateEntityBase(EntityXML* entityXML)
{
	EntityBase* entityBase = new EntityBase();

	entityBase->SetEntityXML(entityXML);
	entityBase->Init();

	int errorOut;
	Model* model = _entityModelXMLCreator->CreateFromFile(entityXML->modelFile, &errorOut);
	if (errorOut == TempEntityModelXMLCreator::NONE)
	{
		entityBase->SetModel(model);
	}

	if (std::find(entityXML->classVector.begin(), entityXML->classVector.end(), "Buildable") != entityXML->classVector.end())
	{
		model = _entityModelXMLCreator->CreateFromFile(entityXML->unbuiltModelFile, &errorOut);
		if (errorOut == TempEntityModelXMLCreator::NONE)
		{
			entityBase->SetUnbuiltModel(model);
		}
	}

	//Phage entity version
	Phage::PhageModel* modelOut = _entityModelXMLCreator->CreatePhageModelFromFile(entityXML->modelFile, &errorOut);
	if (errorOut == TempEntityModelXMLCreator::NONE)
	{
		entityBase->SetPhageModel(modelOut);
	}

	if (std::find(entityXML->classVector.begin(), entityXML->classVector.end(), "Buildable") != entityXML->classVector.end())
	{
		modelOut = _entityModelXMLCreator->CreatePhageModelFromFile(entityXML->unbuiltModelFile, &errorOut);
		if (errorOut == TempEntityModelXMLCreator::NONE)
		{
			entityBase->SetUnbuiltPhageModel(modelOut);
		}
	}

	return entityBase;
}

Model* TempEntityBaseCreator::CreateProp(std::string fileName)
{
	int errorOut;
	Model* model = _entityModelXMLCreator->CreateFromFile(fileName, &errorOut);

	if (errorOut == TempEntityModelXMLCreator::NONE)
		return model;

	return NULL;
}

ProjectileBase* TempEntityBaseCreator::CreateProjectileBase(ProjectileXML* projectileXML)
{
	ProjectileBase* projectileBase = new ProjectileBase();

	projectileBase->SetProjectileXML(projectileXML);
	projectileBase->Init();

	int errorOut;
	Model* model = _entityModelXMLCreator->CreateFromFile(projectileXML->_model, &errorOut);

	if (errorOut == TempEntityModelXMLCreator::NONE)
	{
		projectileBase->SetModel(model);
	}

	return projectileBase;
}

Phage::PhageModel* TempEntityBaseCreator::CreatePropPhage(std::string fileName)
{
	int errorOut;
	Phage::PhageModel* model = _entityModelXMLCreator->CreatePhageModelFromFile(fileName, &errorOut);

	if (errorOut == TempEntityModelXMLCreator::NONE)
		return model;

	return NULL;
}
