#ifndef _GUI_RECTANGLE_H
#define _GUI_RECTANGLE_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/GUI/OrthoDepth.h"

class GUIRectangle
{
public:
	GUIRectangle();
	~GUIRectangle();

	int Draw();

	int SetPoints(glm::vec4 points);
	int SetDepth(int depth);
private:
	float* guiRectangleVertices;
	GLuint* guiRectangleIndices;
	GLuint guiRectangleVAO;
	GLuint guiRectangleVBO;
	GLuint guiRectangleIBO;
};
#endif
