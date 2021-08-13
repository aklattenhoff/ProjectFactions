#ifndef _ENTITY_XML_CREATOR_H
#define _ENTITY_XML_CREATOR_H

#include <string>

class EntityXML;
class EntityXMLParser;

class EntityXMLCreator
{
public:
	EntityXMLCreator();
	~EntityXMLCreator();

	int Init();

	EntityXML* CreateFromFile(std::string fileName);
private:
	EntityXMLParser* _entityXMLParser;
};

#endif
