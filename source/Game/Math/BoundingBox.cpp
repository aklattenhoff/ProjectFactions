#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
}

BoundingBox::~BoundingBox()
{
}

int BoundingBox::SetDimensions(float width, float length, float height)
{
	point1 = glm::vec3(-width/2.0f, 0, -length/2);
	point2 = glm::vec3(width/2.0f, height, length/2);

	return 0;
}

int BoundingBox::SetTranslation(float x, float y, float z)
{
	translation = glm::vec3(x, y, z);

	return 0;
}

int BoundingBox::SetRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);

	return 0;
}

glm::vec3 BoundingBox::GetCenterPoint()
{
	glm::vec3 center = glm::vec3(translation.x, translation.y + (point2.y-point1.y)/2.0f, translation.z);

	return center;
}

int BoundingBox::IntersectRay(glm::vec3 ray1, glm::vec3 ray2, glm::vec3 &intersectionPoint)
{
	//Transform the mouse ray in the negative direction of the box's transformation
	glm::vec3 mouseNT = ray1 - glm::vec3(translation);
	mouseNT = glm::rotate(mouseNT, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 mouseFT = ray2 - glm::vec3(translation);
	mouseFT = glm::rotate(mouseFT, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	//Short-circuit checks
	if (mouseFT.x < point1.x && mouseNT.x < point1.x) return 0;
	if (mouseFT.x > point2.x && mouseNT.x > point2.x) return 0;
	if (mouseFT.y < point1.y && mouseNT.y < point1.y) return 0;
	if (mouseFT.y > point2.y && mouseNT.y > point2.y) return 0;
	if (mouseFT.z < point1.z && mouseNT.z < point1.z) return 0;
	if (mouseFT.z > point2.z && mouseNT.z > point2.z) return 0;

	//Collision check
	if ((GetIntersection(mouseNT, mouseFT, mouseNT.x-point1.x, mouseFT.x-point1.x, intersectionPoint) && InBox(intersectionPoint, point1, point2, 1))
		|| (GetIntersection(mouseNT, mouseFT, mouseNT.y-point1.y, mouseFT.y-point1.y, intersectionPoint) && InBox(intersectionPoint, point1, point2, 2)) 
		|| (GetIntersection(mouseNT, mouseFT, mouseNT.z-point1.z, mouseFT.z-point1.z, intersectionPoint) && InBox(intersectionPoint, point1, point2, 3)) 
		|| (GetIntersection(mouseNT, mouseFT, mouseNT.x-point2.x, mouseFT.x-point2.x, intersectionPoint) && InBox(intersectionPoint, point1, point2, 1)) 
		|| (GetIntersection(mouseNT, mouseFT, mouseNT.y-point2.y, mouseFT.y-point2.y, intersectionPoint) && InBox(intersectionPoint, point1, point2, 2)) 
		|| (GetIntersection(mouseNT, mouseFT, mouseNT.z-point2.z, mouseFT.z-point2.z, intersectionPoint) && InBox(intersectionPoint, point1, point2, 3)))
		return 1;

	return 0;

	return 0;
}

int inline BoundingBox::GetIntersection(glm::vec3 ray1, glm::vec3 ray2, float dst1, float dst2, glm::vec3 &intersectionPoint)
{
	if ((dst1 * dst2) >= 0.0f)
		return 0;
	if (dst1 == dst2)
		return 0;
	intersectionPoint = ray1 + (ray2-ray1) * (-dst1/(dst2-dst1));
	return 1;
}

int inline BoundingBox::InBox(glm::vec3 intersectionPoint, glm::vec3 B1, glm::vec3 B2, const int axis)
{
	if (axis==1 && intersectionPoint.z > B1.z && intersectionPoint.z < B2.z && intersectionPoint.y > B1.y && intersectionPoint.y < B2.y) return 1;
	if (axis==2 && intersectionPoint.z > B1.z && intersectionPoint.z < B2.z && intersectionPoint.x > B1.x && intersectionPoint.x < B2.x) return 1;
	if (axis==3 && intersectionPoint.x > B1.x && intersectionPoint.x < B2.x && intersectionPoint.y > B1.y && intersectionPoint.y < B2.y) return 1;
	return 0;
}
