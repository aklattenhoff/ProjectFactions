#ifndef _ACTION_XML_CREATOR_H
#define _ACTION_XML_CREATOR_H

#include <string>

#include "tinyxml2/tinyxml2.h"

class ActionXML;

class ActionXMLCreator
{
public:
	ActionXMLCreator();
	~ActionXMLCreator();

	int Init();
	ActionXML* CreateFromFile(std::string fileName);
private:
	int ParseBuild(ActionXML* actionXML, tinyxml2::XMLElement* actionElement);
	int ParseTrain(ActionXML* actionXML, tinyxml2::XMLElement* actionElement);
	int ParseBuildWall(ActionXML* actionXML, tinyxml2::XMLElement* actionElement);
	int ParseUpgrade(ActionXML* actionXML, tinyxml2::XMLElement* actionElement);
};
#endif
