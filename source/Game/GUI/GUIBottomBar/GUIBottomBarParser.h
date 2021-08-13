#ifndef _GUI_BOTTOM_BAR_PARSER_H
#define _GUI_BOTTOM_BAR_PARSER_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "tinyxml2/tinyxml2.h"

#include "Texturer/TextureLoader.h"

#include "Game/GUI/GUIBottomBar/GUIBottomBar.h"

class GUIBottomBarParser
{
public:
	GUIBottomBarParser();
	~GUIBottomBarParser();

	int Parse(const char* fileName, GUIBottomBar* guiBottomBar);
private:
};
#endif
