#ifndef _ENTITY_LIST_XML_H
#define _ENTITY_LIST_XML_H

#include <string>
#include <vector>

class EntityXML;

class EntityListXML
{
public:
	EntityListXML();
	~EntityListXML();

	int Init();
	std::vector<EntityXML*> GetEntityXMLVector();
	std::vector<std::string> GetPropFileVector();
	std::vector<std::string> GetProjectileFileVector();
private:
	friend class EntityListXMLCreator;

	std::vector<EntityXML*> _entityXMLVector;
	std::vector<std::string> _propFileVector;
	std::vector<std::string> _projectileFileVector;
};
#endif
