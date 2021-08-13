#ifndef _ACTION_LIST_XML_CREATOR_H
#define _ACTION_LIST_XML_CREATOR_H

#include <string>

#include "tinyxml2/tinyxml2.h"

class ActionXMLCreator;
class ActionListXML;

class ActionListXMLCreator
{
public:
	ActionListXMLCreator();
	~ActionListXMLCreator();

	int Init();
	ActionListXML* CreateFromFile(std::string fileName, int* errorOut = NULL);
private:
	ActionXMLCreator* _actionXMLCreator;
};
#endif
