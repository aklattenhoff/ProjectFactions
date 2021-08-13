#include "ActionArray.h"

ActionArray::ActionArray()
{
}

ActionArray::~ActionArray()
{
}

Action* ActionArray::Get(const char* actionName)
{
	return actionMap[actionName];
}

int ActionArray::Add(Action* action)
{
	actionMap[action->getActionName()] = action;

	return 0;
}

unsigned int ActionArray::Size()
{
	return actionMap.size();
}

std::map<std::string, Action*>::iterator ActionArray::Begin()
{
	return actionMap.begin();
}

std::map<std::string, Action*>::iterator ActionArray::End()
{
	return actionMap.end();
}
