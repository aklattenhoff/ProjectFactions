#include "ProjectileListXML.h"

ProjectileListXML::ProjectileListXML()
{
}

ProjectileListXML::~ProjectileListXML()
{
}

int ProjectileListXML::Init()
{

	return 0;
}

std::vector<ProjectileXML*> ProjectileListXML::GetProjectileXMLVector()
{
	return _projectileXMLVector;
}
