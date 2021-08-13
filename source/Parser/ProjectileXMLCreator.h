#ifndef _PROJECTILE_XML_CREATOR_H
#define _PROJECTILE_XML_CREATOR_H

#include <string>

#include "tinyxml2/tinyxml2.h"

class ProjectileXML;

class ProjectileXMLCreator
{
public:
	ProjectileXMLCreator();
	~ProjectileXMLCreator();

	int Init();
	ProjectileXML* CreateFromFile(std::string fileName);
private:
};
#endif
