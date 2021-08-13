#ifndef _PLANE_H
#define _PLANE_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

class Plane
{
public:
	Plane();
	~Plane();

	int CreateFromPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 GetNormal();
	float GetDistance();
	float GetDistance(glm::vec3 normal);
	glm::vec3 GetSavedPoint();
private:
	glm::vec3 _normal;
	float _distance;
	glm::vec3 _p1;
};
#endif
