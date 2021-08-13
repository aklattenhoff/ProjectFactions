#ifndef _GUI_CURSOR_H
#define _GUI_CURSOR_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/GUI/OrthoDepth.h"

//#include "Graphics/Camera.h"

#include "Renderer/RS/GUITexture.h"

class GUICursor
{
public:
	GUICursor();
	~GUICursor();

	int Init();
	int Draw();
private:
	float* cursorVertices;
	GLuint* cursorIndices;
	float* cursorTexCoords;
	GLuint cursorVAO;
	GLuint cursorVBO;
	GLuint cursorIBO;
	GLuint cursorTBO;
	GLuint texture;

	SDL_Point mousePosition;

	RS::GUITexture _rsGUITexture;
};

#endif
