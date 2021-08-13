#ifndef _ENTITY_XML_TAG_H
#define _ENTITY_XML_TAG_H

#include <string>
#include <vector>

namespace EntityXMLTag
{
	struct EntityTag
	{
		std::string type;
		std::string name;
	};

	typedef std::string Name;
	typedef std::string Class;
	typedef std::string Flag;
	typedef float PopulationCount;
	typedef float BuildLimit;
	typedef float MovementSpeed;
	typedef float MovementTurnRate;
	typedef float MovementCollisionRadius;
	typedef float MovementAwareRadius;
	typedef std::string MovementTerrain;
	typedef float Hitpoints;
	typedef float Trainpoints;
	typedef float Buildpoints;
	typedef float LOS;
	
	typedef std::string ModelFile;
	typedef std::string UnbuiltModelFile;
	typedef std::string SmallIcon;
	typedef std::string MediumIcon;
	typedef std::string LargeIcon;
	typedef float HackArmor;
	typedef float PierceArmor;
	typedef float MagicArmor;

	struct Resource {
		std::string type;
		float resource;
		Resource(std::string t, float r);
	};
	struct Cost {
		std::string type;
		float cost;
		Cost(std::string t, float c);
	};
	struct Action {
		int index;
		std::string actionName;
	};
	struct ClassFloat {
		std::string type;
		float value;
	};
	typedef std::vector<ClassFloat> GatherRateSet;
	typedef std::vector<ClassFloat> GatherRateOverwriteSet;
	struct BuildRate {
		std::vector<ClassFloat> classVector;
	};
	struct BuildRateOverwrite {
		std::vector<ClassFloat> classVector;
	};
	struct Point2D {
		float x;
		float y;
	};
	struct Point3D {
		float x;
		float y;
		float z;
	};
	struct Size2D {
		float width;
	};
	struct Size3D {
		float width;
		float height;
	};
	struct HealthIndicatorPosition {
		Point3D point3D;
	};
	struct BoundingBox2D {
		Point2D pt1;
		Point2D pt2;
		Size2D size;
	};
	struct BoundingBox3D {
		Point3D pt1;
		Point3D pt2;
		Size3D size;
	};
	typedef std::vector<BoundingBox2D> BoundingBox2DSet;
	typedef std::vector<BoundingBox3D> BoundingBox3DSet;

	struct Projectile
	{
		std::string name;
		float frame;
		Point3D point3D;
		float velocity;
	};
	struct Attack {
		std::string type;
		std::string damageType;
		float damage;
		float minRange;
		float maxRange;
		float attackSpeed;
		std::vector<Projectile> projectileVector;
	};

	struct TerrainDecal {
		Point2D pt1;
		Point2D pt2;
		Size2D size;
		std::string texture;
	};
	typedef std::vector<TerrainDecal> TerrainDecalSet;
};

#endif
