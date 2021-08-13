#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Transformation
{
public:
	Transformation();
	~Transformation();
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};
#endif
