#include "ActionSystem.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "ActionManager.h"

ActionSystem::ActionSystem()
{
	_actionManager = NULL;
}

ActionSystem::~ActionSystem()
{
	delete _actionManager;
}

int ActionSystem::Init()
{
	return 0;
}
