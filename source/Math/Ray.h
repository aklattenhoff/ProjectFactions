#ifndef _RAY_H
#define _RAY_H

#include <string>
#include <stack>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

class Ray
{
public:
	Ray();
	~Ray();

	int CreateRayFromTwoPoints(glm::vec3 p1, glm::vec3 p2);

	float CalculateYFromX(float x);
	glm::vec3 CalculatePointFromT(float t);

	int CalculateIntersectTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec4* out);
private:

	glm::vec3 origin;
	glm::vec3 direction;
};
#endif
