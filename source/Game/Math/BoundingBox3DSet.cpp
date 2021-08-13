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
	{
		bb3D.cross.x = 1.0f;
		bb3D.aabb = true;
	}
	else
		bb3D.aabb = false;
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

int BoundingBox3DSet::Size()
{
	return (int)boundingBoxVector.size();
}

const BoundingBox3D* BoundingBox3DSet::GetBoundingBox(int index)
{
	if ((int)boundingBoxVector.size() > index)
		return &boundingBoxVector[index];
	
	return NULL;
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
		if ((GetIntersection(mouseNT, mouseFT, mouseNT.x - point1.x, mouseFT.x - point1.x, intersectionPoint) && InBox(intersectionPoint, point1, point2, 1))
			|| (GetIntersection(mouseNT, mouseFT, mouseNT.y - point1.y, mouseFT.y - point1.y, intersectionPoint) && InBox(intersectionPoint, point1, point2, 2))
			|| (GetIntersection(mouseNT, mouseFT, mouseNT.z - point1.z, mouseFT.z - point1.z, intersectionPoint) && InBox(intersectionPoint, point1, point2, 3))
			|| (GetIntersection(mouseNT, mouseFT, mouseNT.x - point2.x, mouseFT.x - point2.x, intersectionPoint) && InBox(intersectionPoint, point1, point2, 1))
			|| (GetIntersection(mouseNT, mouseFT, mouseNT.y - point2.y, mouseFT.y - point2.y, intersectionPoint) && InBox(intersectionPoint, point1, point2, 2))
			|| (GetIntersection(mouseNT, mouseFT, mouseNT.z - point2.z, mouseFT.z - point2.z, intersectionPoint) && InBox(intersectionPoint, point1, point2, 3)))
		{
			intersectionPoint = glm::rotate(intersectionPoint, glm::radians(boundingBoxVector[i].dot), boundingBoxVector[i].cross);
			intersectionPoint += boundingBoxVector[i].translation;

			intersectionPoint = glm::rotate(intersectionPoint, glm::radians(transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			intersectionPoint += glm::vec3(transformation.translation);

			return 1;
		}
	}

	return 0;
}

int BoundingBox3DSet::IntersectRay(glm::vec3 ray1, glm::vec3 ray2, float &distance, Transformation transformation)
{
	//Transform the mouse ray in the negative direction of the box's transformation
	glm::vec3 mouseNTGlobal = ray1 - glm::vec3(transformation.translation);
	mouseNTGlobal = glm::rotate(mouseNTGlobal, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 mouseFTGlobal = ray2 - glm::vec3(transformation.translation);
	mouseFTGlobal = glm::rotate(mouseFTGlobal, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	distance = -1.0f;

	for (unsigned int i = 0; i < boundingBoxVector.size(); i++)
	{
		glm::vec3 mouseNT = mouseNTGlobal - boundingBoxVector[i].translation;
		glm::vec3 mouseFT = mouseFTGlobal - boundingBoxVector[i].translation;
		if (!boundingBoxVector[i].aabb)
		{
			mouseNT = glm::rotate(mouseNT, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);
			mouseFT = glm::rotate(mouseFT, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);
		}

		glm::vec3 point1 = boundingBoxVector[i].point1;
		glm::vec3 point2 = boundingBoxVector[i].point2;

		glm::vec3 rayDir = glm::normalize(mouseFT - mouseNT);
		glm::vec3 dirFrac = 1.0f / rayDir;

		float t1 = (point1.x - mouseNT.x)*dirFrac.x;
		float t2 = (point2.x - mouseNT.x)*dirFrac.x;
		float t3 = (point1.y - mouseNT.y)*dirFrac.y;
		float t4 = (point2.y - mouseNT.y)*dirFrac.y;
		float t5 = (point1.z - mouseNT.z)*dirFrac.z;
		float t6 = (point2.z - mouseNT.z)*dirFrac.z;

		float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		if (!(tmax < 0.0f) && !(tmin > tmax))
		{
			if (distance < 0.0f || tmin < distance)
				distance = tmin;
		}
	}

	if (distance > 0.0f)
		return 1;

	return 0;
}

int BoundingBox3DSet::IntersectFrustum(Frustum frustum, Transformation transformation)
{
	int returnVal = 2;

	for (unsigned int planeIndex = 2; planeIndex < 6; planeIndex++)
	{
		glm::vec3 planeNormal;
		float planeDistance;
		glm::vec3 planePoint;
		planeNormal = frustum.GetPlane(planeIndex).GetNormal();
		planeNormal = glm::rotate(planeNormal, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

		planePoint = frustum.GetPlane(planeIndex).GetSavedPoint();
		planePoint = planePoint - glm::vec3(transformation.translation);
		planePoint = glm::rotate(planePoint, glm::radians(-transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::vec3 p, n;
		for (unsigned int i = 0; i < boundingBoxVector.size(); i++)
		{
			glm::vec3 planeP = planePoint - boundingBoxVector[i].translation;
			if (!boundingBoxVector[i].aabb)
			{
				planeNormal = glm::rotate(planeNormal, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);
				planeP = glm::rotate(planeP, glm::radians(-boundingBoxVector[i].dot), boundingBoxVector[i].cross);
			}
			planeDistance = glm::dot(-planeNormal, planeP);

			glm::vec3 bMin = boundingBoxVector[i].point1;// +transformation.translation;
			glm::vec3 bMax = boundingBoxVector[i].point2;// +transformation.translation;
			p = bMin;
			n = bMax;
			if (planeNormal.x >= 0.0f) { p.x = bMax.x; n.x = bMin.x; }
			if (planeNormal.y >= 0.0f) { p.y = bMax.y; n.y = bMin.y; }
			if (planeNormal.z >= 0.0f) { p.z = bMax.z; n.z = bMin.z; }

			if (glm::dot(planeNormal, p) + planeDistance < 0.0f)
				return 0;//returnVal = 0;
			else if (glm::dot(planeNormal, n) + planeDistance < 0.0f)
				returnVal = 1;
		}
	}

	return returnVal;
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
