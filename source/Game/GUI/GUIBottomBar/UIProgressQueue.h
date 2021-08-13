#ifndef _UI_PROGRESS_QUEUE_H
#define _UI_PROGRESS_QUEUE_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Input/Input.h"

#include "Game/GUI/OrthoDepth.h"

//#include "Graphics/Camera.h"

#include "Game/GUI/GUIIcon.h"

#include "Game/ActionManager/ActionManagerInterface.h"

#include "Game/World.h"

#include "Renderer/RS/GUITexture.h"
#include "Phage/Phage.h"

class Entity;

class UIProgressQueue
{
public:
	UIProgressQueue();
	~UIProgressQueue();

	int Init();
	int Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity);
	int DrawUIProgressQueue(World* world);
	int ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface);
private:

	glm::vec2 topLeft;
	glm::vec2 botRight;

	GUIIcon* guiIconAction;

	RS::GUITexture _rsGUITexture;
	GLuint _textureID;
};
#endif
