#ifndef _ENTITY_XML_PARSER_H
#define _ENTITY_XML_PARSER_H

#include <string>
#include <vector>

#include "tinyxml2/tinyxml2.h"

#include "Parser/EntityXMLTag.h"

class DebugEntityXMLParser;

class EntityXMLParser
{
public:
	static const int NONE = 0;
	static const int NO_FILE_FOUND = 1;
	static const int NO_ENTITY_FOUND = 2;
	static const int TAG_NOT_FOUND = 3;
public:
	struct FloatTypeTag
	{
		float value;
		std::string type;
		FloatTypeTag(float n, std::string str)
		{
			value = n;
			type = str;
		}
	};
	struct Action
	{
		int row;
		int col;
		std::string name;
	};
	struct Point2D
	{
		float x, y;
	};
	struct Point3D
	{
		float x, y, z;
	};
	struct Size2D
	{
		float width;
	};
	struct Size3D
	{
		float width, height;
	};
	struct BoundingBox2D
	{
		Point2D pt1, pt2;
		Size2D size;
	};
	struct BoundingBox3D
	{
		Point3D pt1, pt2;
		Size3D size;
	};
	struct TerrainDecal
	{
		Point2D pt1, pt2;
		Size2D size;
		std::string texture;
	};
public:
	EntityXMLParser();
	~EntityXMLParser();

	int Init(bool debug);
	int LoadFile(std::string fileName);
	int CloseFile();
	int GetEntityTagInfo(EntityXMLTag::EntityTag* entityTag);

	int GetTagString(std::string tag, std::vector<std::string>* out);
	int GetTagFloat(std::string tag, std::vector<float>* out);

	int GetTagFloatType(std::string tag, std::vector<FloatTypeTag>* out);
	int GetActionVector(std::vector<EntityXMLParser::Action>* out);
	int GetAttack(std::vector<EntityXMLTag::Attack>* out);
	int GetBoundingBox2DSet(std::string tag, std::vector<EntityXMLParser::BoundingBox2D>* out);
	int GetBoundingBox3DSet(std::string tag, std::vector<EntityXMLParser::BoundingBox3D>* out);
	int GetTerrainDecalSet(std::vector<EntityXMLParser::TerrainDecal>* out);
	int GetClassFloatSet(std::string tag, std::vector<EntityXMLTag::ClassFloat>* out);
private:
	tinyxml2::XMLDocument _xmlEntityFile;
	tinyxml2::XMLElement* _xmlElement_Entity;

	DebugEntityXMLParser* _debugEntityXMLParser;
};

#endif
