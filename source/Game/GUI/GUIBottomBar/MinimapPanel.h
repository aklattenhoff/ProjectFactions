#ifndef _MINIMAP_PANEL_H
#define _MINIMAP_PANEL_H

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

#include "Phage/Phage.h"

class MinimapPanel
{
public:
	MinimapPanel();
	~MinimapPanel();

	int Init();
	int Generate(glm::vec2 screenRes, Phage::PhageUIEntity* phageUIEntity);
	int Draw(GLuint tex);
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

	RS::GUITexture _rsGUITexture;
};
#endif
