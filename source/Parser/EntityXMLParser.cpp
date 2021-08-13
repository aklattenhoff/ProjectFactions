#include "EntityXMLParser.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/DebugEntityXMLParser.h"

EntityXMLParser::EntityXMLParser()
{
	_debugEntityXMLParser = NULL;

	//Does not need to be deleted
	_xmlElement_Entity = NULL;
}

EntityXMLParser::~EntityXMLParser()
{
	delete _debugEntityXMLParser;
}

int EntityXMLParser::Init(bool debug)
{
	if (debug)
		_debugEntityXMLParser = new DebugEntityXMLParser();

	return 0;
}

int EntityXMLParser::LoadFile(std::string fileName)
{
	if (_debugEntityXMLParser)
		_debugEntityXMLParser->OpenFile(fileName.c_str());

	if (_xmlEntityFile.LoadFile(fileName.c_str()) != 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoFileFound();
		return NO_FILE_FOUND;
	}

	return NONE;
}

int EntityXMLParser::CloseFile()
{
	_xmlEntityFile.Clear();

	if (_debugEntityXMLParser)
		_debugEntityXMLParser->CloseFile();

	return 0;
}

int EntityXMLParser::GetEntityTagInfo(EntityXMLTag::EntityTag* entityTag)
{
	_xmlElement_Entity = _xmlEntityFile.FirstChildElement("Entity");

	if (!_xmlElement_Entity)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Entity");

		return NO_ENTITY_FOUND;
	}

	if (_xmlElement_Entity->Attribute("type"))
		entityTag->type =  _xmlElement_Entity->Attribute("type");
	else
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoAttributeInTag("type", "Entity");
		return NO_ENTITY_FOUND;
	}

	if (_xmlElement_Entity->Attribute("name"))
		entityTag->name =  _xmlElement_Entity->Attribute("name");
	else
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoAttributeInTag("name", "Entity");
		return NO_ENTITY_FOUND;
	}

	if (_debugEntityXMLParser)
		_debugEntityXMLParser->FoundEntity(entityTag->type.c_str(), entityTag->name.c_str());

	return NONE;
}

int EntityXMLParser::GetTagString(std::string tag, std::vector<std::string>* out)
{
	if (_xmlElement_Entity)
	{
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str());
		while (element)
		{
			out->push_back(element->GetText());
			element = element->NextSiblingElement(tag.c_str());
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound(tag.c_str());
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag(tag.c_str(), out->at(i).c_str());
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetTagFloat(std::string tag, std::vector<float>* out)
{
	if (_xmlElement_Entity)
	{
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str());
		while (element)
		{
			out->push_back((float)atof(element->GetText()));
			element = element->NextSiblingElement(tag.c_str());
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound(tag.c_str());
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag(tag.c_str(), out->at(i));
			}
		}
	}

	return 0;
}

int EntityXMLParser::GetTagFloatType(std::string tag, std::vector<FloatTypeTag>* out)
{
	if (_xmlElement_Entity)
	{
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str());
		while (element)
		{
			out->push_back(FloatTypeTag((float)atof(element->GetText()), element->Attribute("type")));
			element = element->NextSiblingElement(tag.c_str());
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound(tag.c_str());
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTagType(tag.c_str(), out->at(i).value, out->at(i).type.c_str());
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetActionVector(std::vector<EntityXMLParser::Action>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement("Action"))
			return TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("Action");
		while (element)
		{
			EntityXMLParser::Action action;
			action.row = atoi(element->Attribute("row"));
			action.col = atoi(element->Attribute("col"));
			action.name = element->GetText();
			out->push_back(action);
			element = element->NextSiblingElement("Action");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Action");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("Action", 0.0f);
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetAttack(std::vector<EntityXMLTag::Attack>* out)
{
	if (_xmlElement_Entity)
	{
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("Attack");
		while (element)
		{
			out->push_back(EntityXMLTag::Attack());
			out->back().type = element->Attribute("type");
			out->back().damageType = element->FirstChildElement("DamageType")->GetText();
			out->back().damage = (float)atof(element->FirstChildElement("Damage")->GetText());
			out->back().minRange = (float)atof(element->FirstChildElement("MinRange")->GetText());
			out->back().maxRange = (float)atof(element->FirstChildElement("MaxRange")->GetText());
			out->back().attackSpeed = (float)atof(element->FirstChildElement("AttackSpeed")->GetText());

			tinyxml2::XMLElement* projectileElement = element->FirstChildElement("Projectile");
			while (projectileElement)
			{
				out->back().projectileVector.push_back(EntityXMLTag::Projectile());
				out->back().projectileVector.back().name = projectileElement->FirstChildElement("Name")->GetText();
				out->back().projectileVector.back().frame = (float)atof(projectileElement->FirstChildElement("Frame")->GetText());

				tinyxml2::XMLElement* ptElement = projectileElement->FirstChildElement("Point3D");
				out->back().projectileVector.back().point3D.x = (float)atof(ptElement->Attribute("x"));
				out->back().projectileVector.back().point3D.y = (float)atof(ptElement->Attribute("y"));
				out->back().projectileVector.back().point3D.z = (float)atof(ptElement->Attribute("z"));

				out->back().projectileVector.back().velocity = (float)atof(projectileElement->FirstChildElement("Velocity")->GetText());

				projectileElement = projectileElement->NextSiblingElement("Projectile");
			}

			element = element->NextSiblingElement("Attack");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Attack");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("Attack", out->at(i).damageType.c_str());
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetBoundingBox2DSet(std::string tag, std::vector<EntityXMLParser::BoundingBox2D>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement(tag.c_str()))
			return TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str())->FirstChildElement("BoundingBox2D");
		while (element)
		{
			tinyxml2::XMLElement* pt1Element = element->FirstChildElement("Point2D");
			tinyxml2::XMLElement* pt2Element = pt1Element->NextSiblingElement("Point2D");
			tinyxml2::XMLElement* sizeElement = element->FirstChildElement("Size2D");
			if (!pt1Element || !pt2Element || !sizeElement)
				return TAG_NOT_FOUND;
			BoundingBox2D bb2D;
			bb2D.pt1.x = (float)atof(pt1Element->Attribute("x"));
			bb2D.pt1.y = (float)atof(pt1Element->Attribute("y"));
			bb2D.pt2.x = (float)atof(pt2Element->Attribute("x"));
			bb2D.pt2.y = (float)atof(pt2Element->Attribute("y"));
			bb2D.size.width = (float)atof(sizeElement->Attribute("width"));
			out->push_back(bb2D);
			element = element->NextSiblingElement("BoundingBox2D");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("BoundingBox2D");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("BoundingBox2D", 0.0f);
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetBoundingBox3DSet(std::string tag, std::vector<EntityXMLParser::BoundingBox3D>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement(tag.c_str()))
			return TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str())->FirstChildElement("BoundingBox3D");
		while (element)
		{
			tinyxml2::XMLElement* pt1Element = element->FirstChildElement("Point3D");
			tinyxml2::XMLElement* pt2Element = pt1Element->NextSiblingElement("Point3D");
			tinyxml2::XMLElement* sizeElement = element->FirstChildElement("Size3D");
			if (!pt1Element || !pt2Element || !sizeElement)
				return TAG_NOT_FOUND;
			BoundingBox3D bb3D;
			bb3D.pt1.x = (float)atof(pt1Element->Attribute("x"));
			bb3D.pt1.y = (float)atof(pt1Element->Attribute("y"));
			bb3D.pt1.z = (float)atof(pt1Element->Attribute("z"));
			bb3D.pt2.x = (float)atof(pt2Element->Attribute("x"));
			bb3D.pt2.y = (float)atof(pt2Element->Attribute("y"));
			bb3D.pt2.z = (float)atof(pt2Element->Attribute("z"));
			bb3D.size.width = (float)atof(sizeElement->Attribute("width"));
			bb3D.size.height = (float)atof(sizeElement->Attribute("height"));
			out->push_back(bb3D);
			element = element->NextSiblingElement("BoundingBox3D");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("BoundingBox3D");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("BoundingBox3D", 0.0f);
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetTerrainDecalSet(std::vector<EntityXMLParser::TerrainDecal>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement("TerrainDecalSet"))
			return TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("TerrainDecalSet")->FirstChildElement("TerrainDecal");
		while (element)
		{
			tinyxml2::XMLElement* pt1Element = element->FirstChildElement("Point2D");
			tinyxml2::XMLElement* pt2Element = pt1Element->NextSiblingElement("Point2D");
			tinyxml2::XMLElement* sizeElement = element->FirstChildElement("Size2D");
			tinyxml2::XMLElement* textureElement = element->FirstChildElement("Texture");
			if (!pt1Element || !pt2Element || !sizeElement || !textureElement)
				return TAG_NOT_FOUND;
			TerrainDecal terrDec;
			terrDec.pt1.x = (float)atof(pt1Element->Attribute("x"));
			terrDec.pt1.y = (float)atof(pt1Element->Attribute("y"));
			terrDec.pt2.x = (float)atof(pt2Element->Attribute("x"));
			terrDec.pt2.y = (float)atof(pt2Element->Attribute("y"));
			terrDec.size.width = (float)atof(sizeElement->Attribute("width"));
			terrDec.texture = textureElement->GetText();
			out->push_back(terrDec);
			element = element->NextSiblingElement("TerrainDecal");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("TerrainDecal");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("TerrainDecal", 0.0f);
			}
		}
	}

	return NONE;
}

int EntityXMLParser::GetClassFloatSet(std::string tag, std::vector<EntityXMLTag::ClassFloat>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement(tag.c_str()))
			return TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement(tag.c_str())->FirstChildElement("Class");
		while (element)
		{
			EntityXMLTag::ClassFloat classOut;
			classOut.type = element->Attribute("type");
			classOut.value = (float)atof(element->GetText());
			out->push_back(classOut);
			element = element->NextSiblingElement("Class");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Class");
		return TAG_NOT_FOUND;
	}
	else
	{
		if (_debugEntityXMLParser)
		{
			for (unsigned int i = 0;i < out->size();i ++)
			{
				_debugEntityXMLParser->AddedTag("Class", 0.0f);
			}
		}
	}

	return NONE;
}
