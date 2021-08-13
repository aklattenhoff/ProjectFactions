#include "Phage_TerrainTexture.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageTerrainTexture::PhageTerrainTexture()
{
}

Phage::PhageTerrainTexture::~PhageTerrainTexture()
{
}

int Phage::PhageTerrainTexture::Init()
{
	_currentTerrainTextureTile = 0;
	_currentAlphaTextureTile = 0;

	return 0;
}

int Phage::PhageTerrainTexture::CreateTerrainMap(int w, int h)
{
	_width = w;
	_height = h;

	_terrainMap = new unsigned char**[w];
	for (int i = 0;i < w;i ++)
		_terrainMap[i] = new unsigned char*[h];

	for (int j = 0;j < h;j ++)
		for (int i = 0;i < w;i ++)
			_terrainMap[i][j] = new unsigned char[6];

	for (int j = 0;j < h;j ++)
		for (int i = 0;i < w;i ++)
		{
			_terrainMap[i][j][0] = 255;
			_terrainMap[i][j][1] = 255;
			_terrainMap[i][j][2] = 255;
			_terrainMap[i][j][3] = 255;
			_terrainMap[i][j][4] = 255;
			_terrainMap[i][j][5] = 255;
		}

	_testMap = new unsigned char[w*h*4];
	for (int i = 0;i < w*h*4; i ++)
		_testMap[i] = 0;
	for (int i = 0;i < w*h; i ++)
		_testMap[i*4+0] = 0;

	for (int j = 1;j < h-1;j ++)
		for (int i = 1;i < w-1;i ++)
			_testMap[((j+0)*_width+(i+0))*4+0] = 1;

	_testMap2 = new unsigned char[w*h*4];
	for (int i = 0;i < w*h*4; i ++)
		_testMap2[i] = 0;

	//Create terrain map
	SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 24, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));

	int numberOfColors = surface->format->BytesPerPixel;
	GLenum textureFormat;
	if (numberOfColors == 4)
	{
		if (surface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGBA;}
		else
		{textureFormat = GL_BGRA;}
	}
	else if (numberOfColors == 3)
	{
		if (surface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGB;}
		else
		{textureFormat = GL_BGR;}
	}

	glGenTextures(1, &_terrainMap1);
	glBindTexture(GL_TEXTURE_2D, _terrainMap1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _testMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &_terrainMap2);
	glBindTexture(GL_TEXTURE_2D, _terrainMap2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _testMap2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (surface)
		SDL_FreeSurface(surface);

	//Create first terrain tile map and alpha tile map
	SDL_Surface* terrainSurface = SDL_CreateRGBSurface(0, TERRAIN_TILE_MAP_SIZE, TERRAIN_TILE_MAP_SIZE, 32, 0, 0, 0, 0);
	numberOfColors = terrainSurface->format->BytesPerPixel;
	if (numberOfColors == 4)
	{
		if (terrainSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGBA;}
		else
		{textureFormat = GL_BGRA;}
	}
	else if (numberOfColors == 3)
	{
		if (terrainSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGB;}
		else
		{textureFormat = GL_BGR;}
	}

	GLuint terrainTileMap;
	glGenTextures(1, &terrainTileMap);
	glBindTexture(GL_TEXTURE_2D, terrainTileMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terrainSurface->w, terrainSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, terrainSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_terrainTextureTileMapVector.push_back(terrainTileMap);

	GLuint bumpTileMap;
	glGenTextures(1, &bumpTileMap);
	glBindTexture(GL_TEXTURE_2D, bumpTileMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terrainSurface->w, terrainSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, terrainSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_terrainBumpTileMapVector.push_back(bumpTileMap);

	if (terrainSurface)
		SDL_FreeSurface(terrainSurface);

	SDL_Surface* alphaSurface = SDL_CreateRGBSurface(0, ALPHA_TILE_MAP_SIZE, ALPHA_TILE_MAP_SIZE, 32, 0, 0, 0, 0);
	numberOfColors = alphaSurface->format->BytesPerPixel;
	if (numberOfColors == 4)
	{
		if (alphaSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGBA;}
		else
		{textureFormat = GL_BGRA;}
	}
	else if (numberOfColors == 3)
	{
		if (alphaSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGB;}
		else
		{textureFormat = GL_BGR;}
	}

	GLuint alphaTileMap;
	glGenTextures(1, &alphaTileMap);
	glBindTexture(GL_TEXTURE_2D, alphaTileMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, alphaSurface->w, alphaSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, alphaSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_alphaTextureTileMap = alphaTileMap;

	if (alphaSurface)
		SDL_FreeSurface(alphaSurface);

	return 0;
}

int Phage::PhageTerrainTexture::SetTerrainMap(int x, int y, int level, int value)
{
	_testMap[(y*_width+x)*4+level] = value;
	unsigned char data[4] = {_testMap[(y*_width+x)*4+0], _testMap[(y*_width+x)*4+1], _testMap[(y*_width+x)*4+2], _testMap[(y*_width+x)*4+3]};
	data[level] = value;

	glBindTexture(GL_TEXTURE_2D, _terrainMap1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return 0;
}

int Phage::PhageTerrainTexture::SetAlphaMap(int x, int y, int level, int value)
{
	_testMap2[(y*_width+x)*4+level] = value;
	unsigned char data[4] = {_testMap2[(y*_width+x)*4+0], _testMap2[(y*_width+x)*4+1], _testMap2[(y*_width+x)*4+2], _testMap2[(y*_width+x)*4+3]};
	data[level] = value;

	glBindTexture(GL_TEXTURE_2D, _terrainMap2);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return 0;
}

int Phage::PhageTerrainTexture::PaintMap(int x, int y, int value)
{
	x = glm::clamp(x, 1, _width-2);
	y = glm::clamp(y, 1, _height-2); 

	if (ValueOnTop(x, y, value))
		return 1;

	Uint32 texNeighbor = 0;

	//If the top layer is occupied, we have to shift everything down
	if (_testMap[((y+0)*_width+(x+0))*4+2] > 0)
	{
		//_testMap[((y+0)*_width+(x+0))*4+0] = _testMap[((y+0)*_width+(x+0))*4+1];
		_testMap[((y+0)*_width+(x+0))*4+1] = _testMap[((y+0)*_width+(x+0))*4+2];
		//_testMap[((y+0)*_width+(x+0))*4+2] = value;
		_testMap2[((y+0)*_width+(x+0))*4+0] = _testMap2[((y+0)*_width+(x+0))*4+1];
		//_testMap2[((y+0)*_width+(x+0))*4+1] = _testMap2[((y+0)*_width+(x+0))*4+2];
		SetTerrainMap(x, y, 2, value);
	}
	//If the 2nd layer is occupied, we assign our value to the 3rd layer
	else if (_testMap[((y+0)*_width+(x+0))*4+1] > 0)
	{
		//_testMap[((y+0)*_width+(x+0))*4+2] = value;
		SetTerrainMap(x, y, 2, value);
	}
	//If the 1st layer is occupied, we assign our value to the 2nd layer
	else if (_testMap[((y+0)*_width+(x+0))*4+0] > 0)
	{
		//_testMap[((y+0)*_width+(x+0))*4+1] = value;
		SetTerrainMap(x, y, 1, value);
	}
	//If no layer is occupied, then we assign our value to the bottom layer
	else
	{
		//_testMap[((y+0)*_width+(x+0))*4+0] = value;
		SetTerrainMap(x, y, 0, value);
	}

	for (int j = -1;j < 2;j ++)
		for (int i = -1;i < 2;i ++)
		{
			int xa = glm::clamp(x+i, 1, _width-2);
			int ya = glm::clamp(y+j, 1, _height-2);
			if (ValueOnTop(xa, ya, value))
				//_testMap2[((y+j)*_width+(x+i))*4+ActiveLayer(x+i, y+j)-1] = CalculateAlphaMapTexture(CalculateNeighborTexture(x+i, y+j, value));
				SetAlphaMap(xa, ya, ActiveLayer(xa, ya)-1, CalculateAlphaMapTexture(CalculateNeighborTexture(xa, ya, value)));
		}

	return 0;
}

int Phage::PhageTerrainTexture::AddTerrainTexture(std::string fileName, std::string bumpFile)
{
	//Load texture from file
	SDL_Surface *surface = NULL;
	surface = IMG_Load(fileName.c_str());

	if (!surface)
		return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	//Calculate offset for the new texture in the terrain tile map
	int xOffset = (_currentTerrainTextureTile % MAX_TEXTURES_PER_TILE_MAP) * TERRAIN_TEXTURE_SIZE;
	int yOffset = (int)std::floorf((float)(_currentTerrainTextureTile / MAX_TEXTURES_PER_TILE_MAP)) * TERRAIN_TEXTURE_SIZE;

	int width = surface->w < TERRAIN_TEXTURE_SIZE ? surface->w : TERRAIN_TEXTURE_SIZE;
	int height = surface->h < TERRAIN_TEXTURE_SIZE ? surface->h : TERRAIN_TEXTURE_SIZE;

	//Bind tile map and add texture
	glBindTexture(GL_TEXTURE_2D, _terrainTextureTileMapVector.back());
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	
	if (surface)
		SDL_FreeSurface(surface);

	//Bump map load
	surface = NULL;
	if (bumpFile.compare("") == 0)
		surface = IMG_Load("textures/terrain/snow_2.png");
	else
		surface = IMG_Load(bumpFile.c_str());

	if (!surface)
		return -1;

	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	//Calculate offset for the new texture in the terrain tile map
	//int xOffset = (_currentTerrainTextureTile % MAX_TEXTURES_PER_TILE_MAP) * TERRAIN_TEXTURE_SIZE;
	//int yOffset = (int)std::floorf(_currentTerrainTextureTile / MAX_TEXTURES_PER_TILE_MAP) * TERRAIN_TEXTURE_SIZE;

	width = surface->w < TERRAIN_TEXTURE_SIZE ? surface->w : TERRAIN_TEXTURE_SIZE;
	height = surface->h < TERRAIN_TEXTURE_SIZE ? surface->h : TERRAIN_TEXTURE_SIZE;

	//Bind tile map and add texture
	glBindTexture(GL_TEXTURE_2D, _terrainBumpTileMapVector.back());
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	if (surface)
		SDL_FreeSurface(surface);

	//Increment texture count and create new tile map if at the end
	_currentTerrainTextureTile++;
	if (_currentTerrainTextureTile > MAX_TEXTURES_PER_TILE_MAP*MAX_TEXTURES_PER_TILE_MAP)
	{
		CreateBlankTerrainTileMap();
		_currentTerrainTextureTile = 0;
	}

	return 0;
}

int Phage::PhageTerrainTexture::AddAlphaTexture(std::string fileName)
{
	//Load texture from file
	SDL_Surface* surface = NULL;
	IMG_Init(IMG_INIT_PNG);
	surface = IMG_Load(fileName.c_str());

	if (!surface)
		return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	AddAlphaTextureRotation(surface, textureFormat, 0);
	AddAlphaTextureRotation(surface, textureFormat, 1);
	AddAlphaTextureRotation(surface, textureFormat, 2);
	AddAlphaTextureRotation(surface, textureFormat, 3);

	if (surface)
		SDL_FreeSurface(surface);

	return 0;
}

int Phage::PhageTerrainTexture::AddAlphaTextureRotation(SDL_Surface* surface, GLenum textureFormat, int rotation)
{
	SDL_Surface* rotatedSurface = NULL;
	rotatedSurface = CreateRotatedAlphaMap(surface, rotation);

	//Calculate offset for the new texture in the terrain tile map
	int xOffset = (_currentAlphaTextureTile % MAX_ALPHA_PER_TILE_MAP) * ALPHA_TEXTURE_SIZE;
	int yOffset = (int)std::floorf((float)(_currentAlphaTextureTile / MAX_ALPHA_PER_TILE_MAP)) * ALPHA_TEXTURE_SIZE;

	int width = surface->w < ALPHA_TEXTURE_SIZE ? surface->w : ALPHA_TEXTURE_SIZE;
	int height = surface->h < ALPHA_TEXTURE_SIZE ? surface->h : ALPHA_TEXTURE_SIZE;

	//Bind tile map and add texture
	glBindTexture(GL_TEXTURE_2D, _alphaTextureTileMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, textureFormat, GL_UNSIGNED_BYTE, rotatedSurface->pixels);
	
	//if (rotatedSurface)
		//SDL_FreeSurface(rotatedSurface);

	//Increment texture count and create new tile map if at the end
	_currentAlphaTextureTile ++;
	if (_currentAlphaTextureTile > MAX_ALPHA_PER_TILE_MAP*MAX_ALPHA_PER_TILE_MAP)
	{
		_currentAlphaTextureTile = 0;
	}

	return 0;
}

int Phage::PhageTerrainTexture::BindTextures()
{
	glActiveTexture(GL_TEXTURE0+8);
	glBindTexture(GL_TEXTURE_2D, _terrainMap1);
	glActiveTexture(GL_TEXTURE0+9);
	glBindTexture(GL_TEXTURE_2D, _terrainMap2);

	glActiveTexture(GL_TEXTURE0+10);
	glBindTexture(GL_TEXTURE_2D, _alphaTextureTileMap);

	for (int i = 0; i < (int)_terrainTextureTileMapVector.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + 11 + i);
		glBindTexture(GL_TEXTURE_2D, _terrainTextureTileMapVector[i]);
	}

	for (int i = 0; i < (int)_terrainBumpTileMapVector.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + 31 + i);
		glBindTexture(GL_TEXTURE_2D, _terrainBumpTileMapVector[i]);
	}

	glActiveTexture(GL_TEXTURE0);

	return 0;
}

int Phage::PhageTerrainTexture::CreateBlankTerrainTileMap()
{
	//Create new terrain tile map
	SDL_Surface* terrainSurface = SDL_CreateRGBSurface(0, TERRAIN_TILE_MAP_SIZE, TERRAIN_TILE_MAP_SIZE, 32, 0, 0, 0, 0);
	int numberOfColors = terrainSurface->format->BytesPerPixel;
	GLenum textureFormat;
	if (numberOfColors == 4)
	{
		if (terrainSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGBA;}
		else
		{textureFormat = GL_BGRA;}
	}
	else if (numberOfColors == 3)
	{
		if (terrainSurface->format->Rmask == 0x000000ff)
		{textureFormat = GL_RGB;}
		else
		{textureFormat = GL_BGR;}
	}

	GLuint terrainTileMap;
	glGenTextures(1, &terrainTileMap);
	glBindTexture(GL_TEXTURE_2D, terrainTileMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terrainSurface->w, terrainSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, terrainSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_terrainTextureTileMapVector.push_back(terrainTileMap);

	GLuint bumpTileMap;
	glGenTextures(1, &bumpTileMap);
	glBindTexture(GL_TEXTURE_2D, bumpTileMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, terrainSurface->w, terrainSurface->h, 0, textureFormat, GL_UNSIGNED_BYTE, terrainSurface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_terrainBumpTileMapVector.push_back(bumpTileMap);

	if (terrainSurface)
		SDL_FreeSurface(terrainSurface);

	return 0;
}

Uint32 Phage::PhageTerrainTexture::getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;
	case 2:
		return *(Uint16 *)p;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		return *(Uint32 *)p;
		break;
	default:
		return 0;
	}
}

void Phage::PhageTerrainTexture::putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

SDL_Surface* Phage::PhageTerrainTexture::CreateRotatedAlphaMap(SDL_Surface* surface, int rotation)
{
	//Short circuit if there is no rotation needed and simply return the passed surface pointer
	if (rotation == 0)
		return surface;

	SDL_Surface* rotatedSurface = NULL;
	rotatedSurface = SDL_CreateRGBSurface(0,
		rotation == 2 ? surface->w : surface->h,
		rotation == 2 ? surface->h : surface->w,
		surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

	if (rotation == 1)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, (surface->h-1)-j, i, getpixel(surface, i, j));
	}
	else if (rotation == 2)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, (surface->w-1)-i, (surface->h-1)-j, getpixel(surface, i, j));
	}
	else if (rotation == 3)
	{
		for (int i = 0; i < surface->w; i++)
			for (int j = surface->h - 1; j >= 0; j--)
				putpixel(rotatedSurface, j, (surface->w-1)-i, getpixel(surface, i, j));
	}

	return rotatedSurface;
}

int Phage::PhageTerrainTexture::ActiveLayer(int x, int y)
{
	if (_testMap[((y+0)*_width+(x+0))*4+2] != 0)
		return 2;
	else if (_testMap[((y+0)*_width+(x+0))*4+1] != 0)
		return 1;
	else if (_testMap[((y+0)*_width+(x+0))*4+0] != 0)
		return 0;

	return 0;
}

bool Phage::PhageTerrainTexture::ValueOnTop(int x, int y, int value)
{
	if (_testMap[((y+0)*_width+(x+0))*4+2] != 0)
		return (_testMap[((y+0)*_width+(x+0))*4+2] == value);
	else if (_testMap[((y+0)*_width+(x+0))*4+1] != 0)
		return (_testMap[((y+0)*_width+(x+0))*4+1] == value);
	else if (_testMap[((y+0)*_width+(x+0))*4+0] != 0)
		return (_testMap[((y+0)*_width+(x+0))*4+0] == value);

	return false;
}

Uint32 Phage::PhageTerrainTexture::CalculateNeighborTexture(int x, int y, int value)
{
	Uint32 neighborValue = 0x00000000;

	if (ValueOnTop(x-1, y-1, value)) neighborValue |= neighbor_NW;
	if (ValueOnTop(x, y-1, value)) neighborValue |= neighbor_N;
	if (ValueOnTop(x+1, y-1, value)) neighborValue |= neighbor_NE;
	if (ValueOnTop(x-1, y, value)) neighborValue |= neighbor_W;
	if (ValueOnTop(x+1, y, value)) neighborValue |= neighbor_E;
	if (ValueOnTop(x-1, y+1, value)) neighborValue |= neighbor_SW;
	if (ValueOnTop(x, y+1, value)) neighborValue |= neighbor_S;
	if (ValueOnTop(x+1, y+1, value)) neighborValue |= neighbor_SE;

	return neighborValue;
}

int Phage::PhageTerrainTexture::CalculateAlphaMapTexture(Uint32 neighborTex)
{
	//TODO: This is going to need some revisions and optimization - the initial pass is going to be brute force

	int alpha = 0;
	int* alphaID = &alpha;

	(*alphaID) = 1*4;

	//If there is a vertical and a horizontal cardinal, then we need to check corners
	if ((neighborTex & neighbor_N || neighborTex & neighbor_S) && (neighborTex & neighbor_E || neighborTex & neighbor_W))
	{
		//Count the number of cardinal connections
		int cardinalConnections = 0;
		if (neighborTex & neighbor_N) cardinalConnections ++;
		if (neighborTex & neighbor_S) cardinalConnections ++;
		if (neighborTex & neighbor_E) cardinalConnections ++;
		if (neighborTex & neighbor_W) cardinalConnections ++;

		//If 2 connections then it will have a corner connection
		if (cardinalConnections == 2)
		{
			if (neighborTex & neighbor_N && neighborTex & neighbor_E) {
				if (neighborTex & neighbor_NE) (*alphaID) = 5*4;
				else (*alphaID) = 4*4;
			}
			else if (neighborTex & neighbor_N && neighborTex & neighbor_W) {
				if (neighborTex & neighbor_NW) (*alphaID) = 5*4+3;
				else (*alphaID) = 4*4+3;
			}
			else if (neighborTex & neighbor_S && neighborTex & neighbor_E) {
				if (neighborTex & neighbor_SE) (*alphaID) = 5*4+1;
				else (*alphaID) = 4*4+1;
			}
			else if (neighborTex & neighbor_S && neighborTex & neighbor_W) {
				if (neighborTex & neighbor_SW) (*alphaID) = 5*4+2;
				else (*alphaID) = 4*4+2;
			}
		}
		else if (cardinalConnections == 3)
		{
			if (!(neighborTex & neighbor_N)) {
				if (neighborTex & neighbor_SE && neighborTex & neighbor_SW) (*alphaID) = 9*4+1;
				else if (neighborTex & neighbor_SE) (*alphaID) = 7*4+1;
				else if (neighborTex & neighbor_SW) (*alphaID) = 8*4+1;
				else (*alphaID) = 6*4+1;
			}
			else if (!(neighborTex & neighbor_S)) {
				if (neighborTex & neighbor_NE && neighborTex & neighbor_NW) (*alphaID) = 9*4+3;
				else if (neighborTex & neighbor_NE) (*alphaID) = 8*4+3;
				else if (neighborTex & neighbor_NW) (*alphaID) = 7*4+3;
				else (*alphaID) = 6*4+3;
			}
			else if (!(neighborTex & neighbor_E)) {
				if (neighborTex & neighbor_NW && neighborTex & neighbor_SW) (*alphaID) = 9*4+2;
				else if (neighborTex & neighbor_NW) (*alphaID) = 8*4+2;
				else if (neighborTex & neighbor_SW) (*alphaID) = 7*4+2;
				else (*alphaID) = 6*4+2;
			}
			else if (!(neighborTex & neighbor_W)) {
				if (neighborTex & neighbor_NE && neighborTex & neighbor_SE) (*alphaID) = 9*4;
				else if (neighborTex & neighbor_NE) (*alphaID) = 7*4;
				else if (neighborTex & neighbor_SE) (*alphaID) = 8*4;
				else (*alphaID) = 6*4;
			}
		}
		else
		{
			//Count corners
			int cornerConnections = 0;
			if (neighborTex & neighbor_NE) cornerConnections ++;
			if (neighborTex & neighbor_NW) cornerConnections ++;
			if (neighborTex & neighbor_SE) cornerConnections ++;
			if (neighborTex & neighbor_SW) cornerConnections ++;

			if (cornerConnections == 4)
				(*alphaID) = 15*4;
			else if (cornerConnections == 3)
			{
				if (!(neighborTex & neighbor_NE))
					(*alphaID) = 14*4+1;
				else if (!(neighborTex & neighbor_NW))
					(*alphaID) = 14*4;
				else if (!(neighborTex & neighbor_SE))
					(*alphaID) = 14*4+2;
				else if (!(neighborTex & neighbor_SW))
					(*alphaID) = 14*4+3;
			}
			else if (cornerConnections == 2)
			{
				if (neighborTex & neighbor_NE && neighborTex & neighbor_SW)
					(*alphaID) = 13*4;
				else if (neighborTex & neighbor_NW && neighborTex & neighbor_SE)
					(*alphaID) = 13*4+1;
				else if (neighborTex & neighbor_NE && neighborTex & neighbor_SE)
					(*alphaID) = 12*4;
				else if (neighborTex & neighbor_SE && neighborTex & neighbor_SW)
					(*alphaID) = 12*4+1;
				else if (neighborTex & neighbor_NW && neighborTex & neighbor_SW)
					(*alphaID) = 12*4+2;
				else
					(*alphaID) = 12*4+3;
			}
			else
			{
				if (neighborTex & neighbor_NE)
					(*alphaID) = 11*4;
				else if (neighborTex & neighbor_SE)
					(*alphaID) = 11*4+1;
				else if (neighborTex & neighbor_SW)
					(*alphaID) = 11*4+2;
				else
					(*alphaID) = 11*4+3;
			}
		}
	}
	//If there is not a vertical and a horizontal cardinal, then we can ignore corners
	else
	{
		if (!(neighborTex & neighbor_N || neighborTex & neighbor_S || neighborTex & neighbor_E || neighborTex & neighbor_W))
			(*alphaID) = 1*4;
		else if (neighborTex & neighbor_N && neighborTex & neighbor_S)
			(*alphaID) = 3*4;
		else if (neighborTex & neighbor_E && neighborTex & neighbor_W)
			(*alphaID) = 3*4+1;
		else if (neighborTex & neighbor_N)
			(*alphaID) = 2*4;
		else if (neighborTex & neighbor_S)
			(*alphaID) = 2*4+2;
		else if (neighborTex & neighbor_E)
			(*alphaID) = 2*4+1;
		else if (neighborTex & neighbor_W)
			(*alphaID) = 2*4+3;
	}

	return alpha;
}
