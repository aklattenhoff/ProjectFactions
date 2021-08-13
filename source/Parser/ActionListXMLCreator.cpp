#include "ActionListXMLCreator.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Parser/ActionXMLCreator.h"
#include "Parser/ActionListXML.h"

ActionListXMLCreator::ActionListXMLCreator()
{
	_actionXMLCreator = NULL;
}

ActionListXMLCreator::~ActionListXMLCreator()
{
	delete _actionXMLCreator;
}

int ActionListXMLCreator::Init()
{
	_actionXMLCreator = new ActionXMLCreator();
	_actionXMLCreator->Init();

	return 0;
}

ActionListXML* ActionListXMLCreator::CreateFromFile(std::string fileName, int* errorOut)
{
	ActionListXML* actionListXML = NULL;

	tinyxml2::XMLDocument xmlEntityList;
	xmlEntityList.LoadFile(fileName.c_str());

	tinyxml2::XMLElement* actionListElement = NULL;
	if (xmlEntityList.FirstChildElement("ActionList"))
		actionListElement = xmlEntityList.FirstChildElement("ActionList")->FirstChildElement("Include")->FirstChildElement("Action");
	else
	{
		DebugOutputFatalError("No ActionList tag found in action list file: %s\n", fileName.c_str());
		if (errorOut) (*errorOut) = -1;
		return NULL;
	}

	actionListXML = new ActionListXML();
	actionListXML->Init();

	while (actionListElement)
	{
		ActionXML* actionXML = _actionXMLCreator->CreateFromFile(actionListElement->GetText());
		actionListXML->_actionXMLVector.push_back(actionXML);

		actionListElement = actionListElement->NextSiblingElement("Action");
	}

	return actionListXML;
}
