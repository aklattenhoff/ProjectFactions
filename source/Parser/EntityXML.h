#ifndef _ENTITY_XML_H
#define _ENTITY_XML_H

#include "Parser/EntityXMLTag.h"

#include <vector>

class EntityXML
{
public:
	EntityXML();
	~EntityXML();

	int Init();

	EntityXMLTag::EntityTag entityTag;

	EntityXMLTag::Name name;
	std::vector<EntityXMLTag::Class> classVector;
	std::vector<EntityXMLTag::Flag> flagVector;
	std::vector<EntityXMLTag::Resource> resourceVector;
	std::vector<EntityXMLTag::Cost> costVector;

	EntityXMLTag::PopulationCount populationCount;
	EntityXMLTag::BuildLimit buildLimit;

	EntityXMLTag::MovementSpeed movementSpeed;
	EntityXMLTag::MovementTurnRate movementTurnRate;
	EntityXMLTag::MovementCollisionRadius movementCollisionRadius;
	EntityXMLTag::MovementAwareRadius movementAwareRadius;
	std::vector<EntityXMLTag::MovementTerrain> movementTerrainVector;

	EntityXMLTag::Hitpoints hitpoints;
	EntityXMLTag::Trainpoints trainpoints;
	EntityXMLTag::Buildpoints buildpoints;
	EntityXMLTag::LOS los;

	EntityXMLTag::ModelFile modelFile;
	EntityXMLTag::UnbuiltModelFile unbuiltModelFile;
	EntityXMLTag::SmallIcon smallIcon;
	EntityXMLTag::MediumIcon mediumIcon;
	EntityXMLTag::LargeIcon largeIcon;

	std::vector<EntityXMLTag::Action> actionVector;

	EntityXMLTag::HackArmor hackArmor;
	EntityXMLTag::PierceArmor pierceArmor;
	EntityXMLTag::MagicArmor magicArmor;

	std::vector<EntityXMLTag::Attack> attackVector;
	EntityXMLTag::GatherRateSet gatherRateSet;
	EntityXMLTag::GatherRateOverwriteSet gatherRateOverwriteSet;
	EntityXMLTag::BuildRate buildRate;
	EntityXMLTag::BuildRateOverwrite buildRateOverwrite;

	EntityXMLTag::HealthIndicatorPosition healthIndicatorPosition;

	EntityXMLTag::BoundingBox2DSet boundingBox2DSet;
	EntityXMLTag::BoundingBox3DSet boundingBox3DSet;

	EntityXMLTag::BoundingBox2DSet gatherArea;

	EntityXMLTag::TerrainDecalSet terrainDecalSet;
private:
};

#endif
