#ifndef _PROJECTILE_LIST_XML_CREATOR_H
#define _PROJECTILE_LIST_XML_CREATOR_H

#include <string>

#include "tinyxml2/tinyxml2.h"

class ProjectileXMLCreator;
class ProjectileListXML;

class EntityListXML;

class ProjectileListXMLCreator
{
public:
	ProjectileListXMLCreator();
	~ProjectileListXMLCreator();

	int Init();
	ProjectileListXML* CreateFromEntityList(EntityListXML* entityListXML);
private:
	ProjectileXMLCreator* _projectileXMLCreator;
};
#endif
