#ifndef _ACTION_MANAGER_INTERFACE_H
#define _ACTION_MANAGER_INTERFACE_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/ActionManager/Action.h"

class ActionManagerInterface
{
public:
	ActionManagerInterface();
	~ActionManagerInterface();

	int SetActionFromActionPanel(Action* action);
private:
	friend class ActionManager;

	Action* actionFromActionPanel;
};
#endif
