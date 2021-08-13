#ifndef _LOS_H
#define _LOS_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class LOS
{
public:
	typedef std::vector<uint32_t> LOSHorzLine;
	typedef std::vector<LOSHorzLine> LOSPoints;
public:
	LOS();
	~LOS();

	int Init(int w, int h);
	int CreateLOSPoints(float radius, LOSPoints* points);
	int SetLineOfSight(glm::vec2 center, LOSPoints* points);
	int ClearLineOfSight();

	int** GetLOSPointer();
	unsigned int GetLOSTexture();
private:
	int _width;
	int _height;
	int _chunks;

	int** _losMap;

	uint32_t* _losData;
	GLuint _losTexture;

	//Fog of war and discovery map
	bool* fogOfWarMap;
	bool* discoveryMap;
};
#endif
