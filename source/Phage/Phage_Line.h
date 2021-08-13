#ifndef _PHAGE_LINE_H
#define _PHAGE_LINE_H

#include <stdlib.h>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageLine
	{
	public:
		PhageLine();
		~PhageLine();

		int Init();

		int SetPoints(glm::vec3 pt1, glm::vec3 pt2);
		glm::mat4 GetMatrix();

		int Draw();
	private:
		float* _vertices;
		GLuint* _indices;

		GLuint _vao;
		GLuint _vbo;
		GLuint _ibo;

		Phage::Matrix _matrix;
	};
}

#endif
