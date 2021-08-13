#ifndef _OLD_ENTITY_XML_PARSER_H
#define _OLD_ENTITY_XML_PARSER_H

#include <string>
#include <vector>

#include "tinyxml2/tinyxml2.h"

#include "Game/EntityXML/EntityXMLTags.h"
#include "Game/EntityXML/DebugEntityXMLParser.h"

const int EXP_ERROR_NONE = 0;
const int EXP_ERROR_NO_FILE_FOUND = 1;
const int EXP_ERROR_NO_ENTITY_FOUND = 2;
const int EXP_ERROR_TAG_NOT_FOUND = 3;

class OldEntityXMLParser
{
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
public:
	OldEntityXMLParser();
	~OldEntityXMLParser();

	int Init(bool debug);
	int LoadFile(std::string fileName);
	int CloseFile();
	int GetEntityTagInfo(EntityXMLTags::EntityTag* entityTag);

	int GetTagString(std::string tag, std::vector<std::string>* out);
	int GetTagFloat(std::string tag, std::vector<float>* out);

	int GetTagFloatType(std::string tag, std::vector<FloatTypeTag>* out);
	int GetActionVector(std::vector<OldEntityXMLParser::Action>* out);
	int GetAttack(std::vector<EntityXMLTags::Attack>* out);
	int GetBoundingBox2DSet(std::vector<OldEntityXMLParser::BoundingBox2D>* out);
	int GetBoundingBox3DSet(std::vector<OldEntityXMLParser::BoundingBox3D>* out);
private:
	tinyxml2::XMLDocument _xmlEntityFile;
	tinyxml2::XMLElement* _xmlElement_Entity;

	DebugEntityXMLParser* _debugEntityXMLParser;
};

#endif
