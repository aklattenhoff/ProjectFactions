#include "BoundingBox2DSet.h"

BoundingBox2DSet::BoundingBox2DSet()
{
}

BoundingBox2DSet::~BoundingBox2DSet()
{
}

int BoundingBox2DSet::AddBoundingBox(glm::vec2 p1, glm::vec2 p2, float width)
{
	//The bounding box that we are going to add to the set
	BoundingBox2D bb2D;

	//Set point1 and point2
	bb2D.point1 = glm::vec3(p1.x, 0.0f, p1.y);
	bb2D.point2 = glm::vec3(p2.x, 0.0f, p2.y);
	//Set the width and height of the bounding box
	bb2D.width = width;

	//Set the midpoint from the vector
	bb2D.midpoint = (bb2D.point1+bb2D.point2)/2.0f;

	glm::vec3 v1, v2;
	v1 = bb2D.point1 - bb2D.midpoint;
	v2 = glm::normalize(glm::rotate(v1, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f))) * (bb2D.width / 2.0f);
	bb2D.p1Vec = v1 + v2;
	v2 = glm::normalize(glm::rotate(v1, -glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f))) * (bb2D.width / 2.0f);
	bb2D.p2Vec = v1 + v2;

	v1 = bb2D.point2 - bb2D.midpoint;
	v2 = glm::normalize(glm::rotate(v1, -glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f))) * (bb2D.width / 2.0f);
	bb2D.p3Vec = v1 + v2;
	v2 = glm::normalize(glm::rotate(v1, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f))) * (bb2D.width / 2.0f);
	bb2D.p4Vec = v1 + v2;

	DebugOutput("Points: (%f, %f, %f), (%f, %f, %f)\n", bb2D.p1Vec.x, bb2D.p1Vec.y, bb2D.p1Vec.z, bb2D.p2Vec.x, bb2D.p2Vec.y, bb2D.p2Vec.z);
	DebugOutput("Points: (%f, %f, %f), (%f, %f, %f)\n", bb2D.p3Vec.x, bb2D.p3Vec.y, bb2D.p3Vec.z, bb2D.p4Vec.x, bb2D.p4Vec.y, bb2D.p4Vec.z);

	//Push the new bounding box to the vector
	boundingBoxVector.push_back(bb2D);

	return 0;
}
