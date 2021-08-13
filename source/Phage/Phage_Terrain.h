#ifndef _PHAGE_TERRAIN_H
#define _PHAGE_TERRAIN_H

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
	class PhageTerrainMesh;
	class PhageTerrainTexture;
	class PhageTerrainDecal;
	class PhageShader;
	class PhageTerrain
	{
	public:
		PhageTerrain();
		~PhageTerrain();

		int Init();
		int CreateTerrain(int w, int h, int textureScale);

		int CreateAndAddTerrainDecal(std::string fileName, float x, float y, float rotation, float w = -1.0f, float h = -1.0f);

		float CalculateHeight(float x, float y);

		glm::vec3 CalculateMouseOnTerrain(glm::vec3 mouseNear, glm::vec3 mouseZero);
		glm::vec3 GetMouseOnTerrain();
		int PaintPosition(int x, int y, int value);

		glm::ivec2 GetTerrainSize();

		int Render(PhageShader* shader, int** losInfo = NULL, unsigned int losTexture = NULL);
		int RenderTerrainDecals();
	private:
		PhageTerrainMesh* _phageTerrainMesh;
		PhageTerrainTexture* _phageTerrainTexture;
		std::vector<Phage::PhageTerrainDecal*> _phageTerrainDecalVector;
		//Texturer::Texture _grassTexture;
		//Texturer::Texture _grassBumpTexture;
		//Texturer::Texture _grassSBCTexture;

		glm::vec3 _mouseOnTerrain;
	};
}

#endif
