#ifndef _PHAGE_TERRAIN_DECAL_H
#define _PHAGE_TERRAIN_DECAL_H

#include <stdlib.h>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageTerrainDecal
	{
	public:
		PhageTerrainDecal();
		~PhageTerrainDecal();

		int Init();
		int CreateTerrainDecal(std::string fileName, float x, float y, float rotation, float dpi, float w = -1.0f, float h = -1.0f);
		int SetVertexData(std::vector<float>* vertexData, std::vector<float>* normalData, std::vector<float>* tangentData);

		glm::ivec4 GetGridPoints();

		//Phage::Matrix GetMatrix();

		int Draw();
	private:
		Phage::Matrix _matrix;

		GLuint _textureID;
		float* _vertices;
		float* _texCoords;
		float* _normals;
		float* _tangents;
		int _numTris;

		GLuint _vao;
		GLuint _vbo;
		GLuint _tbo;
		GLuint _nbo;
		GLuint _tnbo;

		glm::vec2 _centerPoint;
		glm::vec2 _size;
		float _rotationInRadians;
		glm::vec2 _rotatedPoint[4];
		glm::vec2 _aaBoxPoint[2];
		glm::ivec4 _gridPoint;
	};
}

#endif
