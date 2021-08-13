#include "EntitySelectionHandler.h"

#include "Game/Input/Input.h"
#include "Game/EntitySystem/EntitySelectionList.h"
#include "Game/EntitySystem/EntityMouseOverList.h"
#include "Game/Graphics/Camera.h"
#include "Game/EntitySystem/EntityList.h"
#include "Game/EntitySystem/EntityBase.h"
#include "Game/Math/BoundingBox3DSet.h"

EntitySelectionHandler::EntitySelectionHandler()
{
}

EntitySelectionHandler::~EntitySelectionHandler()
{
}

int EntitySelectionHandler::Init()
{
	return 0;
}

int EntitySelectionHandler::SetOrthoSquare(World* world, glm::vec4* points)
{
	if (points != NULL)
	{
		_entitySelectionFlag = true;

		_entitySelectionPoints.x = points->x;
		_entitySelectionPoints.y = points->y;
		_entitySelectionPoints.z = points->z;
		_entitySelectionPoints.w = points->w;

		if (!world->GetInput()->GetKeyboardInput(INPUT_KEYBOARD_DOWN, SDLK_LSHIFT) &&
			(abs(_entitySelectionPoints.x - _entitySelectionPoints.z) > 10 || abs(_entitySelectionPoints.y - _entitySelectionPoints.w) > 10))
		{
			//Unselect all selected entities
			for (int i = 0;i < world->GetEntitySelectionList()->Size();i ++)
				world->GetEntitySelectionList()->At(i)->SetIsSelected(false);

			world->GetEntitySelectionList()->Clear();
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

	//Queue for determining which object is selected and closest
	std::vector<Entity*> tempSelectedObjects;
	std::vector<float> closestValue;

	Entity* objInst;
	glm::vec3 cameraPos = world->GetCamera()->GetMVPMatrix()->GetViewTranslation();

	//Used to ensure that only one hitbox is selected
	//NOTE: Multiple hitboxes could only be selected if the ray-hitbox intersection happened at the same 3D point for multiple hitboxes
	bool closestDrawn = false;

	int index = 0;
	int size = world->GetEntityList()->Size();
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		//for (int k = 0;k < objInst->GetHitBoxNumber();k ++)
		{
			//if (objInst->GetBoundingBox(k)->IntersectRay(camera->getMouseNear(), camera->getMouseFar(), Hit))
			if (objInst->GetEntityBase()->BoundingBox3D()->IntersectRay(world->GetCamera()->getMouseNear(), world->GetCamera()->getMouseFar(), Hit, objInst->GetTransformation()))
			{
				//Determine distance from hit to camera
				float distance = sqrt(pow(cameraPos.x-(Hit.x+objInst->PosX()), 2.0f)+pow(cameraPos.y-(Hit.y+objInst->PosY()), 2.0f)+pow(cameraPos.z-(Hit.z+objInst->PosZ()), 2.0f));

				if (closest == -1.0f || distance < closest)
				{
					//Push to vectors to be tested later
					//NOTE: This is done so that we can determine the closest hitbox before comparing them
					closest = distance;
					tempSelectedObjects.push_back(objInst);
					closestValue.push_back(distance);
				}
				break;
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

	int index = 0;
	int size = world->GetEntityList()->Size();
	while (index < size)
	{
		objInst = world->GetEntityList()->Get(index);

		//TODO: Fix this so that it's not only looping once!!!
		//for (int k = 0;k < 1;k ++)
		{
			glm::vec3 centerPoint = objInst->GetTransformation().translation + objInst->GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint();
			glm::vec4 point2D = world->GetCamera()->Calculate2DPoint(centerPoint.x, centerPoint.y, centerPoint.z);
			if (point2D.x > point1.x && point2D.y > point1.y && point2D.x < point2.x && point2D.y < point2.y)
			{
				world->GetEntityMouseOverList()->Add(objInst);
				objInst->SetIsMouseOver(true);
			}
			//break;
		}

		index ++;
	}

	return 0;
}
