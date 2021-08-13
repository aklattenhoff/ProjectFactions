#ifndef _PROJECTILE_LIST_XML_H
#define _PROJECTILE_LIST_XML_H

#include <string>
#include <vector>

class ProjectileXML;

class ProjectileListXML
{
public:
	ProjectileListXML();
	~ProjectileListXML();

	int Init();
	std::vector<ProjectileXML*> GetProjectileXMLVector();
private:
	friend class ProjectileListXMLCreator;

	std::vector<ProjectileXML*> _projectileXMLVector;
};
#endif
