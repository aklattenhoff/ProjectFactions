#ifndef _ACTION_LIST_PARSER_H
#define _ACTION_LIST_PARSER_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "tinyxml2/tinyxml2.h"

#include "Game/ActionManager/ActionArray.h"
#include "Game/ActionManager/ActionParser.h"

class ActionListParser
{
public:
	ActionListParser();
	~ActionListParser();

	int ParseActionList(const char* fileName, int factionIndex, ActionArray* actionArray, ActionParser* actionParser);
private:
	tinyxml2::XMLDocument xmlDoc;
};
#endif
