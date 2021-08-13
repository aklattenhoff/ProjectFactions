#include "Transformation.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Transformation::Transformation()
{
	translation = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(0.0f);
}

Transformation::~Transformation()
{
}
