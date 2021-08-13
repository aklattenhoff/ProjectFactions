#ifndef _PHAGE_TERRAIN_MESH_H
#define _PHAGE_TERRAIN_MESH_H

#include <stdlib.h>
#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage_Math.h"

namespace Phage
{
	class PhageTerrainMeshPatch;
	class PhageShader;
	class PhageTerrainMesh
	{
	public:
		PhageTerrainMesh();
		~PhageTerrainMesh();

		int Init();
		int CreateTerrainMesh(int w, int h, int textureScale);
		float GetHeight(int x, int y);
		glm::ivec2 GetSize();
		int GetTextureScale();
		float GetTextureDPI();
		glm::vec3 CalculateMouseOnTerrain(glm::vec3 mouseNear, glm::vec3 mouseZero);
		int GetSubArea(glm::ivec2 p1, glm::ivec2 p2, std::vector<float>* vertices, std::vector<float>* normals, std::vector<float>* tangents);
		int Render(PhageShader* shader, int** losInfo, unsigned int losTexture);
	private:
		const float TERRAIN_MESH_MIN_HEIGHT = -32.0f;
	private:
		int _width;
		int _height;
		int _textureScale;
		float _dpi;
		float* _vertices;
		float* _textureCoords;
		float* _alphaCoords;
		float* _normals;
		float* _smoothNormals;
		float* _tangents;
		float* _bitangents;
		float* _heightMap;

		int _patchScale;
		std::vector<PhageTerrainMeshPatch*> _patchVector;

		int _losLocation;
		int _losTextureSamplerLocation;
	};
}

#endif
