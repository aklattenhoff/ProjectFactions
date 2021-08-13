#include "ActionManagerInterface.h"

ActionManagerInterface::ActionManagerInterface()
{
	actionFromActionPanel = NULL;
}

ActionManagerInterface::~ActionManagerInterface()
{
}

int ActionManagerInterface::SetActionFromActionPanel(Action* action)
{
	actionFromActionPanel = action;

	return 0;
}
