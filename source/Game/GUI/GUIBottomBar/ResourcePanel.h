#ifndef _RESOURCE_PANEL_H
#define _RESOURCE_PANEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Input/Input.h"

#include "Game/GUI/OrthoDepth.h"

#include "Renderer/RS/GUITexture.h"
#include "Renderer/RS/GUIText.h"

#include "Phage/Phage.h"
#include "Game/World.h"

class ResourcePanel
{
public:
	ResourcePanel();
	~ResourcePanel();

	int Init();
	int Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity);
	int Draw(GLuint tex);
	int DrawResources(World* world);
	int ProcessMouse(Input* input);
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
	glm::vec2 size;

	RS::GUITexture _rsGUITexture;
	RS::GUIText _rsGUIText;

	GLuint _foodTexture;
	GLuint _woodTexture;
};
#endif
