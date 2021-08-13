#include "ActionListXML.h"

ActionListXML::ActionListXML()
{
}

ActionListXML::~ActionListXML()
{
}

int ActionListXML::Init()
{

	return 0;
}

std::vector<ActionXML*> ActionListXML::GetActionXMLVector()
{
	return _actionXMLVector;
}
