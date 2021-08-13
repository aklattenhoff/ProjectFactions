#ifndef _ACTION_ARRAY_H
#define _ACTION_ARRAY_H

#include <string>
#include <vector>
#include <map>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/ActionManager/Action.h"

class ActionArray
{
public:
	ActionArray();
	~ActionArray();

	Action* Get(const char* actionName);
	int Add(Action* action);
	unsigned int Size();
	std::map<std::string, Action*>::iterator Begin();
	std::map<std::string, Action*>::iterator End();
private:
	std::map<std::string, Action*> actionMap;
};
#endif
