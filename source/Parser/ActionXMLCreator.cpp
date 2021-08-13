#include "ActionXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/ActionXML.h"

ActionXMLCreator::ActionXMLCreator()
{
}

ActionXMLCreator::~ActionXMLCreator()
{
}

int ActionXMLCreator::Init()
{
	return 0;
}

ActionXML* ActionXMLCreator::CreateFromFile(std::string fileName)
{
	DebugOutput("Parsing action file: %s...\n", fileName);

	tinyxml2::XMLDocument xmlDoc;

	if (xmlDoc.LoadFile(fileName.c_str()) != tinyxml2::XML_NO_ERROR)
		DebugOutputFatalError("Unable to open action file: %s\n", fileName);

	ActionXML* actionXML = new ActionXML();

	tinyxml2::XMLElement* actionElement = xmlDoc.FirstChildElement("Action");

	actionXML->_actionName = actionElement->Attribute("name");
	DebugOutput("    added action name: %s\n", actionXML->_actionName.c_str());
	
	std::string typeString = actionElement->Attribute("type");

	if (typeString.compare("build") == 0)
		actionXML->_actionType = ActionXML::TYPE_BUILD;
	else if (typeString.compare("train") == 0)
		actionXML->_actionType = ActionXML::TYPE_TRAIN;
	else if (typeString.compare("build_wall") == 0)
		actionXML->_actionType = ActionXML::TYPE_BUILD_WALL;
	else if (typeString.compare("upgrade") == 0)
		actionXML->_actionType = ActionXML::TYPE_UPGRADE;

	actionXML->_iconFileName = actionElement->FirstChildElement("Icon")->GetText();

	if (actionXML->_actionType == ActionXML::TYPE_BUILD)
		ParseBuild(actionXML, actionElement);
	else if (actionXML->_actionType == ActionXML::TYPE_TRAIN)
		ParseTrain(actionXML, actionElement);
	else if (actionXML->_actionType == ActionXML::TYPE_BUILD_WALL)
		ParseBuildWall(actionXML, actionElement);
	else if (actionXML->_actionType == ActionXML::TYPE_UPGRADE)
		ParseUpgrade(actionXML, actionElement);
		
	return actionXML;
}

int ActionXMLCreator::ParseBuild(ActionXML* actionXML, tinyxml2::XMLElement* actionElement)
{
	actionXML->_buildObjectName = actionElement->FirstChildElement("Build")->GetText();
	DebugOutput("    build action added: %s\n", actionXML->_buildObjectName.c_str());

	return 0;
}

int ActionXMLCreator::ParseTrain(ActionXML* actionXML, tinyxml2::XMLElement* actionElement)
{
	actionXML->_trainObjectName = actionElement->FirstChildElement("Train")->GetText();
	DebugOutput("    train action added: %s\n", actionXML->_trainObjectName.c_str());

	return 0;
}

int ActionXMLCreator::ParseBuildWall(ActionXML* actionXML, tinyxml2::XMLElement* actionElement)
{
	actionXML->_buildWallConnectorObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_Connector")->GetText();
	actionXML->_buildWall2x1ObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_2x1")->GetText();
	actionXML->_buildWall4x1ObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_4x1")->GetText();
	actionXML->_buildWall6x1ObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_6x1")->GetText();
	DebugOutput("    build wall action added: %s\n", actionXML->_buildWallConnectorObjectName.c_str());

	return 0;
}

int ActionXMLCreator::ParseUpgrade(ActionXML* actionXML, tinyxml2::XMLElement* actionElement)
{
	actionXML->_upgradeObjectName = actionElement->FirstChildElement("Upgrade")->GetText();
	actionXML->_upgradeTime = (float)atof(actionElement->FirstChildElement("Time")->GetText());
	DebugOutput("    upgrade action added: %s\n", actionXML->_trainObjectName.c_str());

	return 0;
}
