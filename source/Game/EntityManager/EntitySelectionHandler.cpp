#include "EntitySelectionHandler.h"

#include "Game/Input/Input.h"
#include "Game/EntityManager/EntitySelectionList.h"
#include "Game/EntityManager/EntityMouseOverList.h"
//#include "Graphics/Camera.h"
#include "Game/Entity/EntityList.h"
#include "Game/Entity/EntityBase.h"
#include "Game/Math/BoundingBox3DSet.h"
#include "Game/Math/Frustum.h"

#include "Renderer/Renderer.h"

EntitySelectionHandler::EntitySelectionHandler()
{
}

EntitySelectionHandler::~EntitySelectionHandler()
{
}

int EntitySelectionHandler::Init()
{
	_entitySelectionFlag = false;

	return 0;
}

int EntitySelectionHandler::SetOrthoSquare(World* world, glm::vec4* points)
{
	if (points != NULL)
	{
		_entitySelectionPoints.x = points->x;
		_entitySelectionPoints.y = points->y;
		_entitySelectionPoints.z = points->z;
		_entitySelectionPoints.w = points->w;

		if (abs(_entitySelectionPoints.x - _entitySelectionPoints.z) > 10 || abs(_entitySelectionPoints.y - _entitySelectionPoints.w) > 10)
		{
			_entitySelectionFlag = true;

			if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
			{

				//Unselect all selected entities
				for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
					world->GetEntitySelectionList()->At(i)->SetIsSelected(false);

				world->GetEntitySelectionList()->Clear();
			}
		}
	}

	return 0;
}

int EntitySelectionHandler::CalcMouseOver(World* world)
{
	ClearMouseOver(world);

	if (!_entitySelectionFlag)
		CalcRayIntersect(world);
	else
		CalcOrthoSquareIntersect(world);

	return 0;
}

int EntitySelectionHandler::SelectMouseOver(World* world)
{
	_entitySelectionFlag = false;

	//Clear previously selected objects
	if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT))
	{
		//Unselect all selected entities
		for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
			world->GetEntitySelectionList()->At(i)->SetIsSelected(false);

		world->GetEntitySelectionList()->Clear();
	}

	//Add mouse over objects to selected objects
	while (world->GetEntityMouseOverList()->Size() > 0)
	{
		world->GetEntityMouseOverList()->Back()->SetIsMouseOver(false);
		if (!world->GetEntityMouseOverList()->Back()->IsSelected())
		{
			world->GetEntityMouseOverList()->Back()->SetIsSelected(true);
			world->GetEntitySelectionList()->Add(world->GetEntityMouseOverList()->Back());
		}
		world->GetEntityMouseOverList()->PopBack();
	}

	return 0;
}

int EntitySelectionHandler::ClearMouseOver(World* world)
{
	while (world->GetEntityMouseOverList()->Size() > 0)
	{
		world->GetEntityMouseOverList()->Back()->SetIsMouseOver(false);
		world->GetEntityMouseOverList()->PopBack();
	}

	return 0;
}

int EntitySelectionHandler::CalcRayIntersect(World* world)
{
	//TEST: Hitbox drawing
	glm::vec3 Hit = glm::vec3(1.0f);
	float closest = -1.0f;
	Entity* closestEntity = NULL;

	//Queue for determining which object is selected and closest
	std::vector<Entity*> tempSelectedObjects;
	std::vector<float> closestValue;

	Entity* objInst;
	//<PhCam>//glm::vec3 cameraPos = RendererSingleton->GetCamera()->GetMVPMatrix()->GetViewTranslation();
	glm::vec3 cameraPos = RendererSingleton->GetPhageCamera()->GetPosition();

	//Used to ensure that only one hitbox is selected
	//NOTE: Multiple hitboxes could only be selected if the ray-hitbox intersection happened at the same 3D point for multiple hitboxes
	bool closestDrawn = false;

	int index = 0;
	int size = world->GetEntityList()->Size();
	float minDistance = -1.0f;
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		if (objInst->GetEntityBase()->BoundingBox3D()->IntersectRay(RendererSingleton->GetMouseNear(), RendererSingleton->GetMouseFar(), minDistance, objInst->GetTransformation()))
		{
			if (closest == -1.0f || minDistance < closest)
			{
				//Push to vectors to be tested later
				//NOTE: This is done so that we can determine the closest hitbox before comparing them
				closest = minDistance;
				closestEntity = objInst;
				RendererSingleton->HoverOverEntity = closestEntity;
			}
		}
		index++;
	}

	if (closestEntity)
	{
		world->GetEntityMouseOverList()->Add(closestEntity);
		closestEntity->SetIsMouseOver(true);
	}

	return 0;

	index = 0;
	size = world->GetEntityList()->Size();
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		//for (int k = 0;k < objInst->GetHitBoxNumber();k ++)
		{
			//if (objInst->GetBoundingBox(k)->IntersectRay(camera->getMouseNear(), camera->getMouseFar(), Hit))
			if (objInst->GetEntityBase()->BoundingBox3D()->IntersectRay(RendererSingleton->GetMouseNear(), RendererSingleton->GetMouseFar(), Hit, objInst->GetTransformation()))
			{
				//Determine distance from hit to camera
				//float distance = sqrt(pow(cameraPos.x-(Hit.x+objInst->PosX()), 2.0f)+pow(cameraPos.y-(Hit.y+objInst->PosY()), 2.0f)+pow(cameraPos.z-(Hit.z+objInst->PosZ()), 2.0f));
				float distance = sqrt(pow(cameraPos.x - (Hit.x), 2.0f) + pow(cameraPos.y - (Hit.y), 2.0f) + pow(cameraPos.z - (Hit.z), 2.0f));

				if (closest == -1.0f || distance < closest)
				{
					//Push to vectors to be tested later
					//NOTE: This is done so that we can determine the closest hitbox before comparing them
					closest = distance;
					tempSelectedObjects.push_back(objInst);
					closestValue.push_back(distance);
					//RendererSingleton->TestPointerVec = Hit;
				}
				//break;
			}
		}

		index ++;
	}

	//Compare object that collided with mouse ray
	//NOTE: These are not all the objects that collided with the ray. They have been culled based on draw order.
		//For example, if an object collided with the ray but was further away than a previously checked object that also collided, the current
		//object would not have been added to the vector
	while (tempSelectedObjects.size() > 0)
	{
		objInst = tempSelectedObjects.back();

		if (!closestDrawn && closest == closestValue.back())
		{
			//This hitbox is the closest 
			world->GetEntityMouseOverList()->Add(tempSelectedObjects.back());
			tempSelectedObjects.back()->SetIsMouseOver(true);
			closestDrawn = true;
		}

		//Remove hit info from vector
		tempSelectedObjects.pop_back();
		closestValue.pop_back();
	}
	//END TEST

	return 0;
}

int EntitySelectionHandler::CalcOrthoSquareIntersect(World* world)
{
	Entity* objInst;

	//Determine the smallest and largest selection point
	glm::vec2 point1 = glm::vec2(0.0f, 0.0f);
	point1.x = _entitySelectionPoints.x < _entitySelectionPoints.z ? _entitySelectionPoints.x : _entitySelectionPoints.z;
	point1.y = _entitySelectionPoints.y < _entitySelectionPoints.w ? _entitySelectionPoints.y : _entitySelectionPoints.w;

	glm::vec2 point2 = glm::vec2(0.0f, 0.0f);
	point2.x = _entitySelectionPoints.x > _entitySelectionPoints.z ? _entitySelectionPoints.x : _entitySelectionPoints.z;
	point2.y = _entitySelectionPoints.y > _entitySelectionPoints.w ? _entitySelectionPoints.y : _entitySelectionPoints.w;

	glm::vec3 nearP1 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point1.x, point1.y, -1.0f));
	glm::vec3 nearP2 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point2.x, point1.y, -1.0f));
	glm::vec3 nearP3 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point2.x, point2.y, -1.0f));
	glm::vec3 nearP4 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point1.x, point2.y, -1.0f));
	glm::vec3 farP1 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point1.x, point1.y, 1.0f));
	glm::vec3 farP2 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point2.x, point1.y, 1.0f));
	glm::vec3 farP3 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point2.x, point2.y, 1.0f));
	glm::vec3 farP4 = glm::vec3(RendererSingleton->GetPhageCamera()->Calculate3DPointFromScreenPos(point1.x, point2.y, 1.0f));
	Frustum frustum;
	frustum.SetNearPlane(nearP1, nearP2, nearP3, nearP4);
	frustum.SetFarPlane(farP1, farP2, farP3, farP4);
	frustum.CreateFrustum();

	//DebugOutput("NearP1: (%f, %f, %f)", nearP1.x, nearP1.y, nearP1.z);
	//RendererSingleton->TestPointerVec = farP1;

	int index = 0;
	int size = world->GetEntityList()->Size();
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		if (objInst->GetEntityBase()->BoundingBox3D()->IntersectFrustum(frustum, objInst->GetTransformation()) > 0)
		{
			world->GetEntityMouseOverList()->Add(objInst);
			objInst->SetIsMouseOver(true);
		}

		index++;
	}

	return 0;

	index = 0;
	size = world->GetEntityList()->Size();
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		glm::vec3 centerPoint = objInst->GetTransformation().translation + objInst->GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint();
		//<PhCam>//glm::vec4 point2D = RendererSingleton->GetCamera()->Calculate2DPoint(centerPoint.x, centerPoint.y, centerPoint.z);
		glm::vec4 point2D = RendererSingleton->GetPhageCamera()->Calculate2DPoint(centerPoint.x, centerPoint.y, centerPoint.z);
		if (point2D.x > point1.x && point2D.y > point1.y && point2D.x < point2.x && point2D.y < point2.y)
		{
			world->GetEntityMouseOverList()->Add(objInst);
			objInst->SetIsMouseOver(true);
		}

		index ++;
	}

	//Convert selection points to frustum
	

	return 0;
}
