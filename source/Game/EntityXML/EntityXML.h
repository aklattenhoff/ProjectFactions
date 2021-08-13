#ifndef _ENTITY_XML_H
#define _ENTITY_XML_H

#include "Game/EntityXML/EntityXMLTags.h"

#include <vector>

class EntityXML
{
public:
	EntityXML();
	~EntityXML();

	int Init();

	EntityXMLTags::EntityTag entityTag;

	EntityXMLTags::Name name;
	std::vector<EntityXMLTags::Class> classVector;
	std::vector<EntityXMLTags::Flag> flagVector;
	std::vector<EntityXMLTags::Resource> resourceVector;
	std::vector<EntityXMLTags::Cost> costVector;

	EntityXMLTags::PopulationCount populationCount;
	EntityXMLTags::BuildLimit buildLimit;

	EntityXMLTags::MovementSpeed movementSpeed;
	EntityXMLTags::MovementTurnRate movementTurnRate;
	EntityXMLTags::MovementCollisionRadius movementCollisionRadius;
	EntityXMLTags::MovementAwareRadius movementAwareRadius;
	std::vector<EntityXMLTags::MovementTerrain> movementTerrainVector;

	EntityXMLTags::Hitpoints hitpoints;
	EntityXMLTags::Trainpoints trainpoints;
	EntityXMLTags::LOS los;

	EntityXMLTags::ModelFile modelFile;
	EntityXMLTags::SmallIcon smallIcon;
	EntityXMLTags::MediumIcon mediumIcon;
	EntityXMLTags::LargeIcon largeIcon;

	std::vector<EntityXMLTags::Action> actionVector;

	EntityXMLTags::HackArmor hackArmor;
	EntityXMLTags::PierceArmor pierceArmor;
	EntityXMLTags::MagicArmor magicArmor;

	std::vector<EntityXMLTags::Attack> attackVector;
	EntityXMLTags::GatherRate gatherRate;
	EntityXMLTags::GatherRateOverwrite gatherRateOverwrite;
	EntityXMLTags::BuildRate buildRate;
	EntityXMLTags::BuildRateOverwrite buildRateOverwrite;

	EntityXMLTags::HealthIndicatorPosition healthIndicatorPosition;

	EntityXMLTags::BoundingBox2DSet boundingBox2DSet;
	EntityXMLTags::BoundingBox3DSet boundingBox3DSet;
private:
};

#endif
