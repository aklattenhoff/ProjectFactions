#ifndef _PHAGE_TERRAIN_TEXTURE_H
#define _PHAGE_TERRAIN_TEXTURE_H

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
	class PhageTerrainTexture
	{
	public:
		PhageTerrainTexture();
		~PhageTerrainTexture();

		int Init();
		int CreateTerrainMap(int w, int h);
		int SetTerrainMap(int x, int y, int level, int value);
		int SetAlphaMap(int x, int y, int level, int value);
		int PaintMap(int x, int y, int value);
		int AddTerrainTexture(std::string fileName, std::string bumpFile = "");
		int AddAlphaTexture(std::string fileName);
		int AddAlphaTextureRotation(SDL_Surface* surface, GLenum textureFormat, int rotation);
		int BindTextures();
	private:
		typedef enum
		{
			neighbor_N = 0x00000001,
			neighbor_S = 0x00000002,
			neighbor_E = 0x00000004,
			neighbor_W = 0x00000008,
			neighbor_NE = 0x00000010,
			neighbor_SE = 0x00000020,
			neighbor_SW = 0x00000040,
			neighbor_NW = 0x00000080,
		} neighborTexture;
	private:
		int CreateBlankTerrainTileMap();
		Uint32 getpixel(SDL_Surface *surface, int x, int y);
		void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		SDL_Surface* CreateRotatedAlphaMap(SDL_Surface* surface, int rotation);
		int ActiveLayer(int x, int y);
		bool ValueOnTop(int x, int y, int value);
		Uint32 CalculateNeighborTexture(int x, int y, int value);
		int CalculateAlphaMapTexture(Uint32 neighborTex);
	private:
		int _width;
		int _height;

		GLuint _terrainMap1;
		GLuint _terrainMap2;
		std::vector<GLuint> _terrainTextureTileMapVector;
		std::vector<GLuint> _terrainBumpTileMapVector;
		GLuint _alphaTextureTileMap;

		unsigned char*** _terrainMap;
		unsigned char* _testMap;
		unsigned char* _testMap2;

		static const int TERRAIN_TILE_MAP_SIZE = 2048;
		static const int TERRAIN_TEXTURE_SIZE = 512;
		static const int MAX_TEXTURES_PER_TILE_MAP = TERRAIN_TILE_MAP_SIZE / TERRAIN_TEXTURE_SIZE;
		static const int ALPHA_TILE_MAP_SIZE = 1024;
		static const int ALPHA_TEXTURE_SIZE = 64;
		static const int MAX_ALPHA_PER_TILE_MAP = ALPHA_TILE_MAP_SIZE / ALPHA_TEXTURE_SIZE;
		int _currentTerrainTextureTile;
		int _currentAlphaTextureTile;
	};
}

#endif
