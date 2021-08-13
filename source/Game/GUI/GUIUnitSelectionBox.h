#ifndef _GUI_UNIT_SELECTION_BOX_H
#define _GUI_UNIT_SELECTION_BOX_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/GUI/OrthoDepth.h"

class GUIUnitSelectionBox
{
public:
	GUIUnitSelectionBox();
	~GUIUnitSelectionBox();

	int Draw();

	int SetPoints(glm::vec4 points);
private:
	float* selectionBoxVertices;
	GLuint* selectionBoxIndices;
	GLuint selectionBoxVAO;
	GLuint selectionBoxVBO;
	GLuint selectionBoxIBO;
};
#endif
