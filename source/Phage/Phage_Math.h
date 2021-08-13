#ifndef _PHAGE_MATH_H
#define _PHAGE_MATH_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	struct vec2
	{
		float x, y;
		vec2();
		vec2(float xi, float yi);
	};

	struct vec3
	{
		float x, y, z;
		vec3();
		vec3(float xi, float yi, float zi);
	};

	class ray
	{
	public:
		ray();
		~ray();

		int CreateRayFromTwoPoints(glm::vec3 p1, glm::vec3 p2);

		float CalculateYFromX(float x);
		glm::vec3 CalculatePointFromT(float t);

		int CalculateIntersectTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec4* out);
	private:

		glm::vec3 origin;
		glm::vec3 direction;
	};
}

#endif
