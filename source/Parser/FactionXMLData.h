#ifndef _FACTION_XML_DATA_H
#define _FACTION_XML_DATA_H

#include <string>

class EntityListXML;
class ActionListXML;
class ProjectileListXML;

class FactionXMLData
{
public:
	FactionXMLData();
	~FactionXMLData();

	int Init();
	EntityListXML* GetEntityListXML();
	ActionListXML* GetActionListXML();
	ProjectileListXML* GetProjectileListXML();
private:
	friend class FactionListXMLParser;

	std::string _name;
	std::string _entityListFileName;
	std::string _actionListFileName;
	EntityListXML* _entityListXML;
	ActionListXML* _actionListXML;
	ProjectileListXML* _projectileListXML;
};
#endif
