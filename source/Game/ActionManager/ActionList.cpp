#include "ActionList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/ActionManager/Action.h"

ActionList::ActionList()
{
}

ActionList::~ActionList()
{
}

int ActionList::Add(Action* action)
{
	_actionVector.push_back(action);
	_actionMap.emplace(action->getActionName(), action);
	
	return 0;
}

Action* ActionList::At(std::string str)
{
	Action* action = NULL;
	try
	{
		action = _actionMap.at(str);
	}
	catch (const std::out_of_range&)
	{
		return NULL;
	}
	return action;
}

int ActionList::Size()
{
	return _actionVector.size();
}
