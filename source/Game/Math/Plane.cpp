#include "Plane.h"

Plane::Plane()
{
}

Plane::~Plane()
{
}

int Plane::CreateFromPoints(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	_normal = glm::normalize(glm::cross(p2 - p1, p3 - p2));
	_distance = glm::dot(_normal, p1);
	_p1 = p1;

	return 0;
}

glm::vec3 Plane::GetNormal()
{
	return _normal;
}

float Plane::GetDistance()
{
	return _distance;
}

float Plane::GetDistance(glm::vec3 normal)
{
	return glm::dot(normal, _p1);
}

glm::vec3 Plane::GetSavedPoint()
{
	return _p1;
}
