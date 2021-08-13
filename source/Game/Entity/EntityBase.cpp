#include "EntityBase.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Phage/Phage.h"

EntityBase::EntityBase()
{
	_entityXML = NULL;
	boundingBox2DSet = new BoundingBox2DSet();
	boundingBox3DSet = new BoundingBox3DSet();
	gatherArea = new BoundingBox2DSet();
	_model = NULL;
	_losPoints = NULL;
}

EntityBase::~EntityBase()
{
	delete _entityXML;
	//delete objectModelXMLParser;
	//delete objectModel;
	delete boundingBox2DSet;
	delete boundingBox3DSet;
	delete gatherArea;
	delete _model;
	delete _losPoints;
}

int EntityBase::Init()
{
	//Load object's model
	//objectModelXMLParser = new ObjectModelXMLParser();
	//objectModelXMLParser->ParseObjectModelXML(_entityXML->modelFile.c_str());

	//objectModel = new ObjectModel();
	//objectModel->LoadObjectScene(objectModelXMLParser->getObjectModelXMLTagVector()[0]->modelFileName.c_str());
	//objectModel->LoadObjectTexture(objectModelXMLParser->getObjectModelXMLTagVector()[0]->textureFileName.c_str());
	//objectModel->LoadObjectBumpTexture(objectModelXMLParser->getObjectModelXMLTagVector()[0]->bumpTextureFileName.c_str());
	//objectModel->LoadObjectSBCTexture(objectModelXMLParser->getObjectModelXMLTagVector()[0]->sbcTextureFileName.c_str());

	return 0;
}

Entity* EntityBase::CreateNewInstance(float x, float y, float rotation, int owningPlayer, float height)
{
	Entity* entity = new Entity();
	Phage::PhageEntity* phageEntity = new Phage::PhageEntity();

	entity->_entityBase = this;
	entity->_transformation.translation.x = x;
	entity->_transformation.translation.y = height;
	entity->_transformation.translation.z = y;
	entity->_transformation.rotation.y = rotation;

	entity->_owningPlayer = owningPlayer;

	if (std::find(_entityXML->classVector.begin(), _entityXML->classVector.end(), "Buildable") != _entityXML->classVector.end())
	{
		entity->_needsBuild = true;
	}

	phageEntity->Init();
	phageEntity->SetPhageModel(_phageModel);
	phageEntity->SetTranslate(x, 0.0f, y);
	if (_phageModel->GetOrientation() == Phage::PhageModel::ORIENTATION_Z)
		phageEntity->SetBaseRotate(-90.0f, 1.0f, 0.0f, 0.0f);
	phageEntity->SetRotate(rotation, 0.0f, 1.0f, 0.0f);
	phageEntity->SetScale(_phageModel->GetScale(), _phageModel->GetScale(), _phageModel->GetScale());
	entity->_phageEntity = phageEntity;

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

	if (_entityXML->entityTag.type.compare("unit") == 0) entityTypeInt = ENTITY_TYPE_UNIT;
	else if (_entityXML->entityTag.type.compare("building") == 0) entityTypeInt = ENTITY_TYPE_BUILDING;
	else if (_entityXML->entityTag.type.compare("object") == 0) entityTypeInt = ENTITY_TYPE_OBJECT;
	else entityTypeInt = ENTITY_TYPE_INVALID;

	for (unsigned int i = 0;i < _entityXML->boundingBox2DSet.size();i ++)
	{
		boundingBox2DSet->AddBoundingBox(	glm::vec2(_entityXML->boundingBox2DSet[i].pt1.x, _entityXML->boundingBox2DSet[i].pt1.y),
											glm::vec2(_entityXML->boundingBox2DSet[i].pt2.x, _entityXML->boundingBox2DSet[i].pt2.y),
											_entityXML->boundingBox2DSet[i].size.width);
	}

	for (unsigned int i = 0;i < _entityXML->boundingBox3DSet.size();i ++)
	{
		boundingBox3DSet->AddBoundingBox(	glm::vec3(_entityXML->boundingBox3DSet[i].pt1.x, _entityXML->boundingBox3DSet[i].pt1.y, _entityXML->boundingBox3DSet[i].pt1.z),
											glm::vec3(_entityXML->boundingBox3DSet[i].pt2.x, _entityXML->boundingBox3DSet[i].pt2.y, _entityXML->boundingBox3DSet[i].pt1.z),
											_entityXML->boundingBox3DSet[i].size.width, _entityXML->boundingBox3DSet[i].size.height);
	}

	for (unsigned int i = 0;i < _entityXML->gatherArea.size();i ++)
	{
		gatherArea->AddBoundingBox(	glm::vec2(_entityXML->gatherArea[i].pt1.x, _entityXML->gatherArea[i].pt1.y),
											glm::vec2(_entityXML->gatherArea[i].pt2.x, _entityXML->gatherArea[i].pt2.y),
											_entityXML->gatherArea[i].size.width);
	}

	//Create class map
	for (unsigned int i = 0;i < eXML->classVector.size();i ++)
	{
		_classMap[eXML->classVector[i]] = true;
	}

	return 0;
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

int EntityBase::SetUnbuiltModel(Model* model)
{
	_unbuiltModel = model;

	return 0;
}

Model* EntityBase::GetUnbuiltModel()
{
	return _unbuiltModel;
}

int EntityBase::SetPhageModel(Phage::PhageModel* phageEntity)
{
	_phageModel = phageEntity;

	return 0;
}

Phage::PhageModel* EntityBase::GetPhageModel()
{
	return _phageModel;
}

int EntityBase::SetUnbuiltPhageModel(Phage::PhageModel* phageEntity)
{
	_unbuiltPhageModel = phageEntity;

	return 0;
}

Phage::PhageModel* EntityBase::GetUnbuiltPhageModel()
{
	return _unbuiltPhageModel;
}

int EntityBase::SetLOSPoints(LOS::LOSPoints* points)
{
	_losPoints = points;

	return 0;
}

LOS::LOSPoints* EntityBase::GetLOSPointsPointer()
{
	return _losPoints;
}

bool EntityBase::ContainsClass(std::string c)
{
	try
	{
		_classMap.at(c);
	}
	catch (const std::out_of_range&)
	{
		return false;
	}
	return true;
}
