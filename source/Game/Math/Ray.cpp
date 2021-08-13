#include "Ray.h"

Ray::Ray()
{
}

Ray::~Ray()
{
}

int Ray::CreateRayFromTwoPoints(glm::vec3 p1, glm::vec3 p2)
{
	origin = p1;
	direction = p2-p1;

	//direction = glm::normalize(direction);

	if (direction.x == 0.0f) direction.x = 0.0001f;
	if (direction.y == 0.0f) direction.y = 0.0001f;
	if (direction.z == 0.0f) direction.z = 0.0001f;

	return 0;
}

float Ray::CalculateYFromX(float x)
{
	float t = (x - origin.x)/direction.x;

	return origin.y + (direction.y*t);
}

glm::vec3 Ray::CalculatePointFromT(float t)
{
	glm::vec3 returnVec;
	returnVec.x = origin.x + (direction.x*t);
	returnVec.y = origin.y + (direction.y*t);
	returnVec.z = origin.z + (direction.z*t);

	return returnVec;
}

int Ray::CalculateIntersectTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec4* out)
{
	glm::vec3 e1 = v1 - v0;
	glm::vec3 e2 = v2 - v0;

	glm::vec3 h = glm::cross(direction, e2);
	float a = glm::dot(e1, h);

	if (a > -0.0001f && a < 0.0001f)
		return -1;

	float f = 1/a;
	glm::vec3 s = origin - v0;
	float u = f * glm::dot(s, h);

	if (u < 0.0f || u > 1.0f)
		return -1;

	glm::vec3 q = glm::cross(s, e1);
	float v = f * glm::dot(direction, q);

	if (v < 0.0f || u + v > 1.0f)
		return -1;

	float t = f * glm::dot(e2, q);

	if (t > 0.0f)
	{
		(*out) = glm::vec4(CalculatePointFromT(t), t);
		return 0;
	}

	return -1;
}
