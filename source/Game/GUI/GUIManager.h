#ifndef _GUI_MANAGER_H
#define _GUI_MANAGER_H

#include <string>
#include <sstream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "Game/World.h"
#include "Game/Input/Input.h"

#include "Game/GUI/GUIUnitSelectionBox.h"
#include "Game/GUI/GUIText.h"
#include "Game/GUI/GUICursor.h"

#include "Game/GUI/GUIBottomBar/GUIBottomBar.h"
#include "Game/GUI/GUIBottomBar/GUIBottomBarParser.h"

//#include "Graphics/Camera.h"

#include "Game/ActionManager/ActionManagerInterface.h"

#include "Game/World.h"

#include "Renderer/RS/GUIText.h"
#include "Renderer/RS/GUILineRect.h"

class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	int Init();
	int PreRenderGUI(glm::vec2 screenRes);
	int Draw(World* world, FactionList* factionList, Camera* camera, bool selecting);
	int Draw(World* world, bool selecting);

	//ProcessMouse processes mouse input and position based actions
	//Returns 1 if the mouse is in GUI region and 0 if it is not
	int ProcessMouse(World* world);

	int SetBottomBarFile(const char* bottomBarFile);

	int SetUnitSelectionBoxPoints(glm::vec4 points);
	int SetFPS(float framesPerSec);

	bool IsMouseOverGUI();
private:
	GUIUnitSelectionBox* guiUnitSelectionBox;
	GUIText* guiText;
	float fps;
	GUICursor* guiCursor;

	GUIBottomBarParser* guiBottomBarParser;
	GUIBottomBar* guiBottomBar;

	int mouseOverGUI;

	RS::GUIText _rsText;
	RS::GUILineRect _rsLineRect;
};
#endif
