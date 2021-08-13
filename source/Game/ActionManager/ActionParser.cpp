#include "ActionParser.h"

ActionParser::ActionParser()
{
}

ActionParser::~ActionParser()
{
}

Action* ActionParser::Parse(const char* fileName, int factionIndex)
{
	DebugOutput("Parsing action file: %s...\n", fileName);

	tinyxml2::XMLDocument xmlDoc;

	if (xmlDoc.LoadFile(fileName) != tinyxml2::XML_NO_ERROR)
		DebugOutputFatalError("Unable to open action file: %s\n", fileName);

	Action* action = new Action();

	tinyxml2::XMLElement* actionElement = xmlDoc.FirstChildElement("Action");

	action->actionName = actionElement->Attribute("name");
	DebugOutput("    added action name: %s\n", action->actionName.c_str());
	
	std::string typeString = actionElement->Attribute("type");

	if (typeString.compare("build") == 0)
		action->actionType = ACTION_TYPE_BUILD;
	else if (typeString.compare("train") == 0)
		action->actionType = ACTION_TYPE_TRAIN;
	else if (typeString.compare("build_wall") == 0)
		action->actionType = ACTION_TYPE_BUILD_WALL;

	action->iconFileName = actionElement->FirstChildElement("Icon")->GetText();
	action->iconTexture = TextureLoader_LoadFromFile(action->iconFileName.c_str());

	if (action->actionType == ACTION_TYPE_BUILD)
		ParseBuild(action, actionElement);
	else if (action->actionType == ACTION_TYPE_TRAIN)
		ParseTrain(action, actionElement);
	else if (action->actionType == ACTION_TYPE_BUILD_WALL)
		ParseBuildWall(action, actionElement);

	return action;
}

int ActionParser::ParseBuild(Action* action, tinyxml2::XMLElement* actionElement)
{
	action->buildObjectName = actionElement->FirstChildElement("Build")->GetText();
	DebugOutput("    build action added: %s\n", action->buildObjectName.c_str());

	return 0;
}

int ActionParser::ParseTrain(Action* action, tinyxml2::XMLElement* actionElement)
{
	action->trainObjectName = actionElement->FirstChildElement("Train")->GetText();
	DebugOutput("    train action added: %s\n", action->buildObjectName.c_str());

	return 0;
}

int ActionParser::ParseBuildWall(Action* action, tinyxml2::XMLElement* actionElement)
{
	action->buildWallConnectorObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_Connector")->GetText();
	action->buildWall4x1ObjectName = actionElement->FirstChildElement("Build_Wall")->FirstChildElement("Wall_4x1")->GetText();
	DebugOutput("    build wall action added: %s\n", action->buildObjectName.c_str());

	return 0;
}
