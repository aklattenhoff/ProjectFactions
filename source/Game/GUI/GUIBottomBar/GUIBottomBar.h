#ifndef _GUI_BOTTOM_BAR_H
#define _GUI_BOTTOM_BAR_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Input/Input.h"

//#include "Graphics/Camera.h"

#include "Game/GUI/GUIBottomBar/ObjectPanel.h"
#include "Game/GUI/GUIBottomBar/ActionPanel.h"
#include "Game/GUI/GUIBottomBar/ResourcePanel.h"
#include "Game/GUI/GUIBottomBar/MinimapPanel.h"
#include "Game/GUI/GUIBottomBar/UIProgressQueue.h"

#include "Game/GUI/GUIIcon.h"

#include "Game/ActionManager/ActionManagerInterface.h"

#include "Game/World.h"

#include "Phage/Phage.h"

class Entity;

struct GUIBottomBarPanel
{
	int position;
	GLuint texture;
};

class GUIBottomBar
{
public:
	GUIBottomBar();
	~GUIBottomBar();

	int Init();
	int GenerateBottomBar(glm::vec2 screenRes);
	int Draw();
	int ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface);
	int DrawEntityAction(World* world, Entity* entity);
	int DrawEntityStats(World* world, Entity* entity);
	int DrawUIProgressQueue(World* world);
	int DrawResources(World* world);
private:
	friend class GUIBottomBarParser;

	GUIBottomBarPanel objectPanelInfo;
	GUIBottomBarPanel actionPanelInfo;
	GUIBottomBarPanel resourcePanelInfo;
	GUIBottomBarPanel minimapPanelInfo;

	Phage::PhageUIEntity* _phageObjectPanel;
	Phage::PhageUIEntity* _phageActionPanel;
	Phage::PhageUIEntity* _phageResourcePanel;
	Phage::PhageUIEntity* _phageMinimapPanel;
	Phage::PhageUIEntity* _phageUIProgressQueue;

	ObjectPanel* objectPanel;
	ActionPanel* actionPanel;
	ResourcePanel* resourcePanel;
	MinimapPanel* minimapPanel;
	UIProgressQueue* uiProgressQueue;

	GUIIcon* guiIcon;
};
#endif
