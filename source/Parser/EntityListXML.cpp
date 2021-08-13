#include "EntityListXML.h"

EntityListXML::EntityListXML()
{
}

EntityListXML::~EntityListXML()
{
}

int EntityListXML::Init()
{

	return 0;
}

std::vector<EntityXML*> EntityListXML::GetEntityXMLVector()
{
	return _entityXMLVector;
}

std::vector<std::string> EntityListXML::GetPropFileVector()
{
	return _propFileVector;
}

std::vector<std::string> EntityListXML::GetProjectileFileVector()
{
	return _projectileFileVector;
}
