#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

int Frustum::SetNearPlane(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, glm::vec3 botLeft)
{
	_nearPlanePoints[0] = topLeft;
	_nearPlanePoints[1] = topRight;
	_nearPlanePoints[2] = botRight;
	_nearPlanePoints[3] = botLeft;

	return 0;
}

int Frustum::SetFarPlane(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 botRight, glm::vec3 botLeft)
{
	_farPlanePoints[0] = topLeft;
	_farPlanePoints[1] = topRight;
	_farPlanePoints[2] = botRight;
	_farPlanePoints[3] = botLeft;

	return 0;
}

int Frustum::CreateFrustum()
{
	_near.CreateFromPoints(_nearPlanePoints[0], _nearPlanePoints[1], _nearPlanePoints[2]);
	_far.CreateFromPoints(_farPlanePoints[1], _farPlanePoints[0], _farPlanePoints[3]);
	_left.CreateFromPoints(_nearPlanePoints[0], _nearPlanePoints[3], _farPlanePoints[3]);
	_top.CreateFromPoints(_nearPlanePoints[1], _nearPlanePoints[0], _farPlanePoints[0]);
	_right.CreateFromPoints(_nearPlanePoints[2], _nearPlanePoints[1], _farPlanePoints[2]);
	_bot.CreateFromPoints(_nearPlanePoints[3], _nearPlanePoints[2], _farPlanePoints[2]);

	return 0;
}

Plane Frustum::GetPlane(int index)
{
	switch (index)
	{
	case 0: return _near; break;
	case 1: return _far; break;
	case 2: return _left; break;
	case 3: return _top; break;
	case 4: return _right; break;
	case 5: return _bot; break;
	default: break;
	}

	return _near;
}
