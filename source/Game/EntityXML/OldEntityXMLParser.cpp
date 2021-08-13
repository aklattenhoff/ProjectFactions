#include "OldEntityXMLParser.h"

OldEntityXMLParser::OldEntityXMLParser()
{
	_debugEntityXMLParser = NULL;

	//Does not need to be deleted
	_xmlElement_Entity = NULL;
}

OldEntityXMLParser::~OldEntityXMLParser()
{
	delete _debugEntityXMLParser;
}

int OldEntityXMLParser::Init(bool debug)
{
	if (debug)
		_debugEntityXMLParser = new DebugEntityXMLParser();

	return 0;
}

int OldEntityXMLParser::LoadFile(std::string fileName)
{
	if (_debugEntityXMLParser)
		_debugEntityXMLParser->OpenFile(fileName.c_str());

	if (_xmlEntityFile.LoadFile(fileName.c_str()) != 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoFileFound();
		return EXP_ERROR_NO_FILE_FOUND;
	}

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::CloseFile()
{
	_xmlEntityFile.Clear();

	if (_debugEntityXMLParser)
		_debugEntityXMLParser->CloseFile();

	return 0;
}

int OldEntityXMLParser::GetEntityTagInfo(EntityXMLTags::EntityTag* entityTag)
{
	_xmlElement_Entity = _xmlEntityFile.FirstChildElement("Entity");

	if (!_xmlElement_Entity)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Entity");

		return EXP_ERROR_NO_ENTITY_FOUND;
	}

	if (_xmlElement_Entity->Attribute("type"))
		entityTag->type =  _xmlElement_Entity->Attribute("type");
	else
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoAttributeInTag("type", "Entity");
		return EXP_ERROR_NO_ENTITY_FOUND;
	}

	if (_xmlElement_Entity->Attribute("name"))
		entityTag->name =  _xmlElement_Entity->Attribute("name");
	else
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoAttributeInTag("name", "Entity");
		return EXP_ERROR_NO_ENTITY_FOUND;
	}

	if (_debugEntityXMLParser)
		_debugEntityXMLParser->FoundEntity(entityTag->type.c_str(), entityTag->name.c_str());

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetTagString(std::string tag, std::vector<std::string>* out)
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetTagFloat(std::string tag, std::vector<float>* out)
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

int OldEntityXMLParser::GetTagFloatType(std::string tag, std::vector<FloatTypeTag>* out)
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetActionVector(std::vector<OldEntityXMLParser::Action>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement("Action"))
			return EXP_ERROR_TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("Action");
		while (element)
		{
			OldEntityXMLParser::Action action;
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetAttack(std::vector<EntityXMLTags::Attack>* out)
{
	if (_xmlElement_Entity)
	{
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("Attack");
		while (element)
		{
			out->push_back(EntityXMLTags::Attack());
			out->back().type = element->Attribute("type");
			out->back().damageType = element->FirstChildElement("DamageType")->GetText();
			out->back().damage = (float)atof(element->FirstChildElement("Damage")->GetText());
			out->back().minRange = (float)atof(element->FirstChildElement("MinRange")->GetText());
			out->back().maxRange = (float)atof(element->FirstChildElement("MaxRange")->GetText());
			out->back().attackSpeed = (float)atof(element->FirstChildElement("AttackSpeed")->GetText());
			element = element->NextSiblingElement("Attack");
		}
	}

	if (out->size() <= 0)
	{
		if (_debugEntityXMLParser)
			_debugEntityXMLParser->NoTagFound("Attack");
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetBoundingBox2DSet(std::vector<OldEntityXMLParser::BoundingBox2D>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement("BoundingBox2DSet"))
			return EXP_ERROR_TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("BoundingBox2DSet")->FirstChildElement("BoundingBox2D");
		while (element)
		{
			tinyxml2::XMLElement* pt1Element = element->FirstChildElement("Point2D");
			tinyxml2::XMLElement* pt2Element = pt1Element->NextSiblingElement("Point2D");
			tinyxml2::XMLElement* sizeElement = element->FirstChildElement("Size2D");
			if (!pt1Element || !pt2Element || !sizeElement)
				return EXP_ERROR_TAG_NOT_FOUND;
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}

int OldEntityXMLParser::GetBoundingBox3DSet(std::vector<OldEntityXMLParser::BoundingBox3D>* out)
{
	if (_xmlElement_Entity)
	{
		if (!_xmlElement_Entity->FirstChildElement("BoundingBox3DSet"))
			return EXP_ERROR_TAG_NOT_FOUND;
		tinyxml2::XMLElement* element = _xmlElement_Entity->FirstChildElement("BoundingBox3DSet")->FirstChildElement("BoundingBox3D");
		while (element)
		{
			tinyxml2::XMLElement* pt1Element = element->FirstChildElement("Point3D");
			tinyxml2::XMLElement* pt2Element = pt1Element->NextSiblingElement("Point3D");
			tinyxml2::XMLElement* sizeElement = element->FirstChildElement("Size3D");
			if (!pt1Element || !pt2Element || !sizeElement)
				return EXP_ERROR_TAG_NOT_FOUND;
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
		return EXP_ERROR_TAG_NOT_FOUND;
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

	return EXP_ERROR_NONE;
}
