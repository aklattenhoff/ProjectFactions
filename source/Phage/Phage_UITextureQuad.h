#ifndef _PHAGE_UI_TEXTURE_QUAD_H
#define _PHAGE_UI_TEXTURE_QUAD_H

#include <stdlib.h>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageUITextureQuad
	{
	public:
		PhageUITextureQuad();
		~PhageUITextureQuad();

		int CreateTextureQuad(float x, float y, float w, float h, std::string fileName, bool upsideDown = false);
		int CreateTextureQuad(float x, float y, float w, float h, GLuint textureID, bool upsideDown = false);
		int SetTextureID(GLuint textureID);
		Phage::Matrix GetMatrix();

		int Draw();
	private:
		Phage::Matrix _matrix;

		GLuint _textureID;

		float* _vertices;
		GLuint* _indices;
		float* _texCoords;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;
		GLuint _tbo;
	};
}

#endif
