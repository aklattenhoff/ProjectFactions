#ifndef _PHAGE_UI_LINE_RECT_H
#define _PHAGE_UI_LINE_RECT_H

#include <stdlib.h>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageUILineRect
	{
	public:
		PhageUILineRect();
		~PhageUILineRect();

		int CreateLineRect();

		int Draw(glm::vec4 points);
	private:
		GLuint _textureID;
		float* selectionBoxVertices;
		GLuint* selectionBoxIndices;
		float* selectionBoxTexCoords;
		GLuint selectionBoxVAO;
		GLuint selectionBoxVBO;
		GLuint selectionBoxIBO;
		GLuint selectionBoxTBO;
	};
}

#endif
