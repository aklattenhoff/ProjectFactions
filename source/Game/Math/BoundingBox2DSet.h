#ifndef _BOUNDING_BOX_2D_SET_H
#define _BOUNDING_BOX_2D_SET_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Transformation.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

//A single 2D bounding box description
struct BoundingBox2D
{
	//Smallest point of original vector
	glm::vec3 point1;
	//Largest point of original vector
	glm::vec3 point2;
	//The width of the bounding box
	float width;

	//The midpoint of the original vector
	glm::vec3 midpoint;

	//p1v-------p3v ---
	//  | \   / |    |
	// p1---m---p2   width
	//  | /   \ |    |
	//p2v-------p4v ---
	glm::vec3 p1Vec;
	glm::vec3 p2Vec;
	glm::vec3 p3Vec;
	glm::vec3 p4Vec;
};

class BoundingBox2DSet
{
public:
	BoundingBox2DSet();
	~BoundingBox2DSet();

	//Adds a bounding box given a middle length vector (described by two 2D points) and a width
	int AddBoundingBox(glm::vec2 p1, glm::vec2 p2, float width);

	unsigned Size(){return boundingBoxVector.size();}
	BoundingBox2D Get(int index){return boundingBoxVector[index];}
private:
	std::vector<BoundingBox2D> boundingBoxVector;
};
#endif
