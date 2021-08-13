#ifndef _BOUNDING_BOX_3D_SET_H
#define _BOUNDING_BOX_3D_SET_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Transformation.h"
#include "Game/Graphics/Camera.h"

#include "Game/ObjectManager/Model/BoundingBoxModel.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

//A single 3D bounding box description
struct BoundingBox3D
{
	//Smallest point of axis-aligned box
	glm::vec3 point1;
	//Largest point of axis-aligned box
	glm::vec3 point2;
	//The translation from the origin to the first Point3D
	glm::vec3 translation;
	//The cross product of the second Point3D and the axis-aligned vector
	glm::vec3 cross;
	//The dot product of the second Point3D and the axis-aligned vector
	float dot;
	//The width and height of the bounding box
	glm::vec2 size;

	//The midpoint from the original vector line
	glm::vec3 midpoint;
};

class BoundingBox3DSet
{
public:
	BoundingBox3DSet();
	~BoundingBox3DSet();

	//Adds a bounding box given a middle length vector (described by two 3D points), a width, and a height
	int AddBoundingBox(glm::vec3 p1, glm::vec3 p2, float width, float height);

	int DrawBoundingBox(Camera* camera, BoundingBoxModel* bbModel, Transformation transformation);

	glm::vec3 CalcAverageMidpoint();

	//Checks whether the bounding box intersects the ray and sets the passed intersection point
	int IntersectRay(glm::vec3 ray1, glm::vec3 ray2, glm::vec3 &intersectionPoint, Transformation transformation);
private:
	int inline GetIntersection(glm::vec3 ray1, glm::vec3 ray2, float dst1, float dst2, glm::vec3 &intersectionPoint);
	int inline InBox(glm::vec3 intersectionPoint, glm::vec3 B1, glm::vec3 B2, const int axis);
private:

	std::vector<BoundingBox3D> boundingBoxVector;
};
#endif
