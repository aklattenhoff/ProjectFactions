#ifndef _ENTITY_LIST_XML_CREATOR_H
#define _ENTITY_LIST_XML_CREATOR_H

#include <string>

class EntityXMLCreator;
class EntityListXML;

class EntityListXMLCreator
{
public:
	EntityListXMLCreator();
	~EntityListXMLCreator();

	int Init();
	EntityListXML* CreateFromFile(std::string fileName, int* errorOut = NULL);
private:
	EntityXMLCreator* _entityXMLCreator;
};
#endif
