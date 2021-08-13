#ifndef _PHAGE_TERRAIN_TILE_DEBUG_H
#define _PHAGE_TERRAIN_TILE_DEBUG_H

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
	class PhageTerrainTileDebug
	{
	public:
		PhageTerrainTileDebug();
		~PhageTerrainTileDebug();

		int Init();

		int SetTranslate(float x, float y, float z);
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
