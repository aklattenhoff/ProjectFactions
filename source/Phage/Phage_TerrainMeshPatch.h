#ifndef _PHAGE_TERRAIN_MESH_PATCH_H
#define _PHAGE_TERRAIN_MESH_PATCH_H

#include <stdlib.h>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class PhageTerrainMeshPatch
	{
	public:
		PhageTerrainMeshPatch();
		~PhageTerrainMeshPatch();

		int Init();
		int CreatePatch(int w, int h, std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> smoothNormals, std::vector<float> tangents, std::vector<float> alphaCoords);
		int Render();
	private:
		GLuint _vao;
		GLuint _vbo;
		GLuint _tbo;
		GLuint _nbo;
		GLuint _abo;
		GLuint _tnbo;

		int _width;
		int _height;
	};
}

#endif
