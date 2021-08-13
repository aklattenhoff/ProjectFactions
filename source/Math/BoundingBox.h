#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

#include <string>
#include <stack>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	//Sets the smallest and largest point of bounding box
	int SetDimensions(float width, float length, float height);

	//Sets the translation vector of the bounding box
	int SetTranslation(float x, float y, float z);

	//Sets the rotation vector of the bounding box
	int SetRotation(float x, float y, float z);

	//Returns the center point of the bounding box
	//NOTE: This ignores rotation, so if bounding box is not rotated around it's center, this point will be incorrect
	glm::vec3 GetCenterPoint();

	//Checks whether the bounding box intersects the ray and sets the passed intersection point
	int IntersectRay(glm::vec3 ray1, glm::vec3 ray2, glm::vec3 &intersectionPoint);
private:
	int inline GetIntersection(glm::vec3 ray1, glm::vec3 ray2, float dst1, float dst2, glm::vec3 &intersectionPoint);
	int inline InBox(glm::vec3 intersectionPoint, glm::vec3 B1, glm::vec3 B2, const int axis);
private:
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 translation;
	glm::vec3 rotation;
};
#endif
