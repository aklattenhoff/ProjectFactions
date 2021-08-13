#ifndef _ACTION_PARSER_H
#define _ACTION_PARSER_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "tinyxml2/tinyxml2.h"

#include "Game/Graphics/TextureLoader.h"

#include "Game/ActionManager/Action.h"

class ActionParser
{
public:
	ActionParser();
	~ActionParser();

	Action* Parse(const char* fileName, int factionIndex);
private:
	int ParseBuild(Action* action, tinyxml2::XMLElement* actionElement);
	int ParseTrain(Action* action, tinyxml2::XMLElement* actionElement);
};
#endif
