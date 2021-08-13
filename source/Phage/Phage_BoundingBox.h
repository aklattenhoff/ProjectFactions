#ifndef _PHAGE_BOUNDING_BOX_H
#define _PHAGE_BOUNDING_BOX_H

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
	class PhageBoundingBox
	{
	public:
		PhageBoundingBox();
		~PhageBoundingBox();

		int Init();

		int SetTranslate(float x, float y, float z);
		int SetScale(float x, float y, float z);
		int SetRotate(float a);
		int Translate(float x, float y, float z);
		int Rotate(float a, float x, float z, float y);
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
