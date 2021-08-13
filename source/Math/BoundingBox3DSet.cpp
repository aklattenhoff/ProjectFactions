#include "BoundingBox3DSet.h"

BoundingBox3DSet::BoundingBox3DSet()
{
}

BoundingBox3DSet::~BoundingBox3DSet()
{
}

int BoundingBox3DSet::AddBoundingBox(glm::vec3 p1, glm::vec3 p2, float width, float height)
{
	//The bounding box that we are going to add to the set
	BoundingBox3D bb3D;

	//We simply translate the bounding box so that p1 is at the origin
	bb3D.translation = p1;
	//Set the width and height of the bounding box
	bb3D.size = glm::vec2(width, height);
	//Set the midpoint from the vector
	bb3D.midpoint = (p1+p2)/2.0f;

	//The point is p2 translated by negative p1.
	//This is because we translated p1 to the origin, so we need to translate p2 by the same
	glm::vec3 point = p2 - p1;

	//Compute the cross vector for the +x-axis and the point
	bb3D.cross = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(point));
	if (bb3D.cross.x == 0.0f && bb3D.cross.y == 0.0f && bb3D.cross.z == 0.0f)
		bb3D.cross.x = 1.0f;
	//Compute the dot angle value for the +x-axis and the point
	bb3D.dot = glm::acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(point)))*180.0f/3.1415926f;
	//Rotate the point by the negative angle about the cross vector so that we get the axis-aligned point
	glm::vec3 axisAlignedP2 = glm::rotate(point, glm::radians(-bb3D.dot), bb3D.cross);

	//Since the length vector is now from the origin (0, 0, 0) to axis-aligned point (axisAlignedPoint.x, 0, 0),
	//the smallest and largest points will always be the same
	bb3D.point1 = glm::vec3(0.0f, -height/2.0f, -width/2.0f);
	bb3D.point2 = glm::vec3(axisAlignedP2.x, height/2.0f, width/2.0f);

	//Push the new bounding box to the vector
	boundingBoxVector.push_back(bb3D);

	return 0;
}

int BoundingBox3DSet::DrawBoundingBox(Camera* camera, BoundingBoxModel* bbModel, Transformation transformation)
{
	for (unsigned int i = 0;i < boundingBoxVector.size();i ++)
	{
		camera->GetMVPMatrix()->Push(MODEL);
		camera->GetMVPMatrix()->translate(MODEL, transformation.translation.x, transformation.translation.y, transformation.translation.z);
		camera->GetMVPMatrix()->rotate(MODEL, transformation.rotation.y, 0.0f, 1.0f, 0.0f);
		camera->GetMVPMatrix()->translate(MODEL, boundingBoxVector[i].translation.x, boundingBoxVector[i].translation.y, boundingBoxVector[i].translation.z);
		camera->GetMVPMatrix()->rotate(MODEL, boundingBoxVector[i].dot, boundingBoxVector[i].cross.x, boundingBoxVector[i].cross.y, boundingBoxVector[i].cross.z);
		camera->UpdateMatrix(SHADER_NO_TEXTURE);
		bbModel->DrawBoundingBoxModelUpdate(boundingBoxVector[i].size.x, boundingBoxVector[i].point2.x, boundingBoxVector[i].size.y);
		camera->GetMVPMatrix()->Pop(MODEL);
	}

	return 0;
}

glm::vec3 BoundingBox3DSet::CalcAverageMidpoint()
{
	glm::vec3 avg = glm::vec3(0.0f);

	for (unsigned int i = 0;i < boundingBoxVector.size();i ++)
		avg += boundingBoxVector[i].midpoint;

	avg /= (float)boundingBoxVector.size();

	return avg;
}

int BoundingBox3DSet::IntersectRay(glm::vec3 ray1, glm::vec3 ray2, glm::vec3 &intersectionPoint, Transformation transformation)
{
	//Transform the mouse ray in the negative direction of the box's transformation
	glm::vec3 mouseNTGlobal = ray1 - glm::vec3(transformation.translation);
	mouseNTGlobal = glm::rotate(mouseNTGlobal, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 mouseFTGlobal = ray2 - glm::vec3(transformation.translation);
	mouseFTGlobal = glm::rotate(mouseFTGlobal, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	for (unsigned int i = 0;i < boundingBoxVector.size();i ++)
	{
		glm::vec3 mouseNT = mouseNTGlobal - boundingBoxVector[i].translation;
		mouseNT = glm::rotate(mouseNT, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);

		glm::vec3 mouseFT = mouseFTGlobal - boundingBoxVector[i].translation;
		mouseFT = glm::rotate(mouseFT, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);

		glm::vec3 point1 = boundingBoxVector[i].point1;
		glm::vec3 point2 = boundingBoxVector[i].point2;

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
	}

	return 0;
}

int inline BoundingBox3DSet::GetIntersection(glm::vec3 ray1, glm::vec3 ray2, float dst1, float dst2, glm::vec3 &intersectionPoint)
{
	if ((dst1 * dst2) >= 0.0f)
		return 0;
	if (dst1 == dst2)
		return 0;
	intersectionPoint = ray1 + (ray2-ray1) * (-dst1/(dst2-dst1));
	return 1;
}

int inline BoundingBox3DSet::InBox(glm::vec3 intersectionPoint, glm::vec3 B1, glm::vec3 B2, const int axis)
{
	if (axis==1 && intersectionPoint.z > B1.z && intersectionPoint.z < B2.z && intersectionPoint.y > B1.y && intersectionPoint.y < B2.y) return 1;
	if (axis==2 && intersectionPoint.z > B1.z && intersectionPoint.z < B2.z && intersectionPoint.x > B1.x && intersectionPoint.x < B2.x) return 1;
	if (axis==3 && intersectionPoint.x > B1.x && intersectionPoint.x < B2.x && intersectionPoint.y > B1.y && intersectionPoint.y < B2.y) return 1;
	return 0;
}
