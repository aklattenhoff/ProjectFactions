#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Plane.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

class Frustum
{
public:
	Frustum();
	~Frustum();

	int SetNearPlane(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, glm::vec3 botLeft);
	int SetFarPlane(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, glm::vec3 botLeft);
	int CreateFrustum();
	Plane GetPlane(int index);
private:
	glm::vec3 _nearPlanePoints[4];
	glm::vec3 _farPlanePoints[4];
	Plane _near, _far, _left, _top, _right, _bot;
};
#endif
