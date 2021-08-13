#include "EntityBaseCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/EntityXML.h"
#include "Parser/EntityXMLTag.h"

EntityBaseCreator::EntityBaseCreator()
{
	_entityXMLParser = NULL;
}

EntityBaseCreator::~EntityBaseCreator()
{
	delete _entityXMLParser;
}

int EntityBaseCreator::Init()
{
	_entityXMLParser = new EntityXMLParser();
	_entityXMLParser->Init(true);

	return 0;
}

EntityBase* EntityBaseCreator::CreateFromFile(const char* fileName, int factionIndex)
{
	_entityXMLParser->LoadFile(fileName);

	//Create and allocate EntityBase
	EntityBase* entityBase = new EntityBase();
	entityBase->factionIndex = factionIndex;

	EntityXML* entityXML = new EntityXML();
	entityXML->Init();

	_entityXMLParser->GetEntityTagInfo(&entityXML->entityTag);
	if (entityXML->entityTag.type.compare("unit") == 0) entityBase->entityTypeInt = ENTITY_TYPE_UNIT;
	else if (entityXML->entityTag.type.compare("building") == 0) entityBase->entityTypeInt = ENTITY_TYPE_BUILDING;
	else if (entityXML->entityTag.type.compare("object") == 0) entityBase->entityTypeInt = ENTITY_TYPE_OBJECT;
	else entityBase->entityTypeInt = ENTITY_TYPE_INVALID;

	std::vector<std::string> stringVector;
	std::vector<float> floatVector;
	std::vector<EntityXMLParser::FloatTypeTag> floatTypeTagVector;
	std::vector<EntityXMLParser::Action> actionVector;
	std::vector<EntityXMLParser::BoundingBox2D> bb2DVector;
	std::vector<EntityXMLParser::BoundingBox3D> bb3DVector;
	std::vector<EntityXMLTag::Attack> attackVector;

	if (_entityXMLParser->GetTagString("Name", &stringVector) == EntityXMLParser::NONE)
		entityXML->name = stringVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("Class", &stringVector) == EntityXMLParser::NONE)
		entityXML->classVector = stringVector;

	stringVector.clear();
	if (_entityXMLParser->GetTagString("Flag", &stringVector) == EntityXMLParser::NONE)
		entityXML->flagVector = stringVector;

	if (_entityXMLParser->GetTagFloatType("Resource", &floatTypeTagVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < floatTypeTagVector.size();i ++)
			entityXML->resourceVector.push_back(EntityXMLTag::Resource(floatTypeTagVector[i].type, floatTypeTagVector[i].value));
	}

	floatTypeTagVector.clear();
	if (_entityXMLParser->GetTagFloatType("Cost", &floatTypeTagVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < floatTypeTagVector.size();i ++)
			entityXML->costVector.push_back(EntityXMLTag::Cost(floatTypeTagVector[i].type, floatTypeTagVector[i].value));
	}

	if (_entityXMLParser->GetTagFloat("PopulationCount", &floatVector) == EntityXMLParser::NONE)
		entityXML->populationCount = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("BuildLimit", &floatVector) == EntityXMLParser::NONE)
		entityXML->buildLimit = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("MovementSpeed", &floatVector) == EntityXMLParser::NONE)
		entityXML->movementSpeed = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("MovementTurnRate", &floatVector) == EntityXMLParser::NONE)
		entityXML->movementTurnRate = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("MovementCollisionRadius", &floatVector) == EntityXMLParser::NONE)
		entityXML->movementCollisionRadius = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("MovementAwareRadius", &floatVector) == EntityXMLParser::NONE)
		entityXML->movementAwareRadius = floatVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("MovementTerrain", &stringVector) == EntityXMLParser::NONE)
		entityXML->movementTerrainVector = stringVector;

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("Hitpoints", &floatVector) == EntityXMLParser::NONE)
		entityXML->hitpoints = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("Trainpoints", &floatVector) == EntityXMLParser::NONE)
		entityXML->trainpoints = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("LOS", &floatVector) == EntityXMLParser::NONE)
		entityXML->los = floatVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("ModelFile", &stringVector) == EntityXMLParser::NONE)
		entityXML->modelFile = stringVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("SmallIcon", &stringVector) == EntityXMLParser::NONE)
		entityXML->smallIcon = stringVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("MediumIcon", &stringVector) == EntityXMLParser::NONE)
		entityXML->mediumIcon = stringVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("LargeIcon", &stringVector) == EntityXMLParser::NONE)
		entityXML->largeIcon = stringVector[0];

	//Actions
	if (_entityXMLParser->GetActionVector(&actionVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < actionVector.size();i ++)
		{
			entityXML->actionVector.push_back(EntityXMLTag::Action());
			entityXML->actionVector.back().index = actionVector[i].row*8 + actionVector[i].col;
			entityXML->actionVector.back().actionName = actionVector[i].name;
		}
	}

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("HackArmor", &floatVector) == EntityXMLParser::NONE)
		entityXML->hackArmor = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("PierceArmor", &floatVector) == EntityXMLParser::NONE)
		entityXML->pierceArmor = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("MagicArmor", &floatVector) == EntityXMLParser::NONE)
		entityXML->magicArmor = floatVector[0];

	if (_entityXMLParser->GetAttack(&attackVector) == EntityXMLParser::NONE)
		entityXML->attackVector = attackVector;

	//GatherRate
	//GatherRateOverwrite
	//BuildRate
	//BuildRateOverwrite
	//HealthIndicatorPosition

	/*if (_entityXMLParser->GetBoundingBox2DSet("BoundingBox2DSet", &bb2DVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < bb2DVector.size();i ++)
			entityBase->boundingBox2DSet->AddBoundingBox(	glm::vec2(bb2DVector[i].pt1.x, bb2DVector[i].pt1.y),
															glm::vec2(bb2DVector[i].pt2.x, bb2DVector[i].pt2.y),
															bb2DVector[i].size.width);
	}
	if (_entityXMLParser->GetBoundingBox3DSet("BoundingBox3DSet", &bb3DVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < bb3DVector.size();i ++)
			entityBase->boundingBox3DSet->AddBoundingBox(	glm::vec3(bb3DVector[i].pt1.x, bb3DVector[i].pt1.y, bb3DVector[i].pt1.z),
															glm::vec3(bb3DVector[i].pt2.x, bb3DVector[i].pt2.y, bb3DVector[i].pt1.z),
															bb3DVector[i].size.width, bb3DVector[i].size.height);
	}*/

	_entityXMLParser->CloseFile();
	entityBase->SetEntityXML(entityXML);

	//Needs to be called after variables are set
	entityBase->Init();

	return entityBase;
}
