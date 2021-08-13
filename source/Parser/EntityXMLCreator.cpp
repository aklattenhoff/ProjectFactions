#include "EntityXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/EntityXML.h"
#include "Parser/EntityXMLParser.h"

#include "tinyxml2/tinyxml2.h"

#include "Parser/EntityXMLTag.h"

EntityXMLCreator::EntityXMLCreator()
{
	_entityXMLParser = NULL;
}

EntityXMLCreator::~EntityXMLCreator()
{
	delete _entityXMLParser;
}

int EntityXMLCreator::Init()
{
	_entityXMLParser = new EntityXMLParser();
	_entityXMLParser->Init(true);

	return 0;
}

EntityXML* EntityXMLCreator::CreateFromFile(std::string fileName)
{
	_entityXMLParser->LoadFile(fileName);

	EntityXML* entityXML = new EntityXML();
	entityXML->Init();

	_entityXMLParser->GetEntityTagInfo(&entityXML->entityTag);

	std::vector<std::string> stringVector;
	std::vector<float> floatVector;
	std::vector<EntityXMLParser::FloatTypeTag> floatTypeTagVector;
	std::vector<EntityXMLParser::Action> actionVector;
	std::vector<EntityXMLParser::BoundingBox2D> bb2DVector;
	std::vector<EntityXMLParser::BoundingBox3D> bb3DVector;
	std::vector<EntityXMLParser::TerrainDecal> terrDecVector;
	std::vector<EntityXMLTag::Attack> attackVector;
	std::vector<EntityXMLTag::ClassFloat> classFloatVector;

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
	if (_entityXMLParser->GetTagFloat("Buildpoints", &floatVector) == EntityXMLParser::NONE)
		entityXML->buildpoints = floatVector[0];

	floatVector.clear();
	if (_entityXMLParser->GetTagFloat("LOS", &floatVector) == EntityXMLParser::NONE)
		entityXML->los = floatVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("ModelFile", &stringVector) == EntityXMLParser::NONE)
		entityXML->modelFile = stringVector[0];

	stringVector.clear();
	if (_entityXMLParser->GetTagString("UnbuiltModelFile", &stringVector) == EntityXMLParser::NONE)
		entityXML->unbuiltModelFile = stringVector[0];

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
	classFloatVector.clear();
	if (_entityXMLParser->GetClassFloatSet("GatherRate", &classFloatVector) == EntityXMLParser::NONE)
		entityXML->gatherRateSet = classFloatVector;
	//GatherRateOverwrite
	classFloatVector.clear();
	if (_entityXMLParser->GetClassFloatSet("GatherRateOverwrite", &classFloatVector) == EntityXMLParser::NONE)
		entityXML->gatherRateOverwriteSet = classFloatVector;
	//BuildRate
	//BuildRateOverwrite
	//HealthIndicatorPosition

	if (_entityXMLParser->GetBoundingBox2DSet("BoundingBox2DSet", &bb2DVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < bb2DVector.size();i ++)
		{
			entityXML->boundingBox2DSet.push_back(EntityXMLTag::BoundingBox2D());
			entityXML->boundingBox2DSet.back().pt1.x = bb2DVector[i].pt1.x;
			entityXML->boundingBox2DSet.back().pt1.y = bb2DVector[i].pt1.y;
			entityXML->boundingBox2DSet.back().pt2.x = bb2DVector[i].pt2.x;
			entityXML->boundingBox2DSet.back().pt2.y = bb2DVector[i].pt2.y;
			entityXML->boundingBox2DSet.back().size.width = bb2DVector[i].size.width;
		}
		/*for (unsigned int i = 0;i < bb2DVector.size();i ++)
			entityBase->boundingBox2DSet->AddBoundingBox(	glm::vec2(bb2DVector[i].pt1.x, bb2DVector[i].pt1.y),
															glm::vec2(bb2DVector[i].pt2.x, bb2DVector[i].pt2.y),
															bb2DVector[i].size.width);*/
	}
	if (_entityXMLParser->GetBoundingBox3DSet("BoundingBox3DSet", &bb3DVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < bb3DVector.size();i ++)
		{
			entityXML->boundingBox3DSet.push_back(EntityXMLTag::BoundingBox3D());
			entityXML->boundingBox3DSet.back().pt1.x = bb3DVector[i].pt1.x;
			entityXML->boundingBox3DSet.back().pt1.y = bb3DVector[i].pt1.y;
			entityXML->boundingBox3DSet.back().pt1.z = bb3DVector[i].pt1.z;
			entityXML->boundingBox3DSet.back().pt2.x = bb3DVector[i].pt2.x;
			entityXML->boundingBox3DSet.back().pt2.y = bb3DVector[i].pt2.y;
			entityXML->boundingBox3DSet.back().pt2.z = bb3DVector[i].pt2.z;
			entityXML->boundingBox3DSet.back().size.width = bb3DVector[i].size.width;
			entityXML->boundingBox3DSet.back().size.height = bb3DVector[i].size.height;
		}
		/*for (unsigned int i = 0;i < bb3DVector.size();i ++)
			entityBase->boundingBox3DSet->AddBoundingBox(	glm::vec3(bb3DVector[i].pt1.x, bb3DVector[i].pt1.y, bb3DVector[i].pt1.z),
															glm::vec3(bb3DVector[i].pt2.x, bb3DVector[i].pt2.y, bb3DVector[i].pt1.z),
															bb3DVector[i].size.width, bb3DVector[i].size.height);*/
	}

	//GatherArea
	bb2DVector.clear();
	if (_entityXMLParser->GetBoundingBox2DSet("GatherArea", &bb2DVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < bb2DVector.size();i ++)
		{
			entityXML->gatherArea.push_back(EntityXMLTag::BoundingBox2D());
			entityXML->gatherArea.back().pt1.x = bb2DVector[i].pt1.x;
			entityXML->gatherArea.back().pt1.y = bb2DVector[i].pt1.y;
			entityXML->gatherArea.back().pt2.x = bb2DVector[i].pt2.x;
			entityXML->gatherArea.back().pt2.y = bb2DVector[i].pt2.y;
			entityXML->gatherArea.back().size.width = bb2DVector[i].size.width;
		}
	}

	if (_entityXMLParser->GetTerrainDecalSet(&terrDecVector) == EntityXMLParser::NONE)
	{
		for (unsigned int i = 0;i < terrDecVector.size();i ++)
		{
			entityXML->terrainDecalSet.push_back(EntityXMLTag::TerrainDecal());
			entityXML->terrainDecalSet.back().pt1.x = terrDecVector[i].pt1.x;
			entityXML->terrainDecalSet.back().pt1.y = terrDecVector[i].pt1.y;
			entityXML->terrainDecalSet.back().pt2.x = terrDecVector[i].pt2.x;
			entityXML->terrainDecalSet.back().pt2.y = terrDecVector[i].pt2.y;
			entityXML->terrainDecalSet.back().size.width = terrDecVector[i].size.width;
			entityXML->terrainDecalSet.back().texture = terrDecVector[i].texture;
		}
	}

	_entityXMLParser->CloseFile();

	return entityXML;
}
