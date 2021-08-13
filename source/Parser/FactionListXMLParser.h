#ifndef _FACTION_LIST_XML_PARSER_H
#define _FACTION_LIST_XML_PARSER_H

#include <string>

class FactionListXML;
class EntityListXMLCreator;
class ActionListXMLCreator;
class ProjectileListXMLCreator;

class FactionListXMLParser
{
public:
	FactionListXMLParser();
	~FactionListXMLParser();

	int Init();
	int Parse(std::string fileName);
	FactionListXML* GetFactionListXML();
private:
	FactionListXML* _factionListXML;
	EntityListXMLCreator* _entityListXMLCreator;
	ActionListXMLCreator* _actionListXMLCreator;
	ProjectileListXMLCreator* _projectileListXMLCreator;
};
#endif
