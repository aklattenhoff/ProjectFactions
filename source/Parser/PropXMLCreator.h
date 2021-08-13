#ifndef _PROP_XML_CREATOR_H
#define _PROP_XML_CREATOR_H

#include <string>

#include "tinyxml2/tinyxml2.h"

class PropXML;

class PropXMLCreator
{
public:
	PropXMLCreator();
	~PropXMLCreator();

	int Init();
	PropXML* CreateFromFile(std::string fileName);
private:
};
#endif
