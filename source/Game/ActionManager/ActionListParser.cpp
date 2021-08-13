#include "ActionListParser.h"

ActionListParser::ActionListParser()
{
}

ActionListParser::~ActionListParser()
{
}

int ActionListParser::ParseActionList(const char* fileName, int factionIndex, ActionArray* actionArray, ActionParser* actionParser)
{
	xmlDoc.LoadFile(fileName);

	tinyxml2::XMLElement* actionElement = xmlDoc.FirstChildElement("ActionList")->FirstChildElement("Include")->FirstChildElement("Action");

	actionArray->Add(actionParser->Parse(actionElement->GetText(), factionIndex));

	while (actionElement->NextSiblingElement("Action"))
	{
		actionElement = actionElement->NextSiblingElement("Action");

		actionArray->Add(actionParser->Parse(actionElement->GetText(), factionIndex));
	}

	return 0;
}
