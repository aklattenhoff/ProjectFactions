#ifndef _PHAGE_QUAD_H
#define _PHAGE_QUAD_H

#include <stdlib.h>
#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageQuad
	{
	public:
		PhageQuad();
		~PhageQuad();

		int CreateTextureQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, std::string fileName, bool upsideDown = false);
		int CreateTextureQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, GLuint textureID, bool upsideDown = false);
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
