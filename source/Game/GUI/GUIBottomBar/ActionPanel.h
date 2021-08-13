#ifndef _ACTION_PANEL_H
#define _ACTION_PANEL_H

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

class ActionPanel
{
public:
	ActionPanel();
	~ActionPanel();

	int Init();
	int Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity);
	int Draw(GLuint tex);
	int DrawEntityActions(World* world, Entity* entity);
	int ProcessMouse(World* world, Input* input, ActionManagerInterface* actionManagerInterface);
private:
	float* vertices;
	GLuint* indices;
	float* texCoords;
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLuint TBO;
	GLuint texture;

	glm::vec2 topLeft;
	glm::vec2 botRight;

	GUIIcon* guiIconAction;

	RS::GUITexture _rsGUITexture;
};
#endif
