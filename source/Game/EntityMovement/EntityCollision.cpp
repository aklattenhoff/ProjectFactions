#include "EntityCollision.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityStats.h"
#include "Game/EntityManager/EntityMap.h"

#include "Game/World.h"
#include "Game/PathFinder/PathFinder.h"

//Temp: just to get things working
//#include "Game/Terrain/Terrain.h"
#include "Renderer/Renderer.h"

EntityCollision::EntityCollision()
{
}

EntityCollision::~EntityCollision()
{
}

int EntityCollision::Init()
{
	return 0;
}

int EntityCollision::StartCollisionCheck(World* world, Entity* entity)
{
	entity->_collisionVector = glm::vec3(0.0f, 0.0f, 0.0f);
	entity->_collisionNumber = 0;
	entity->_collisionWithMap = false;
	entity->_collisionMapVector = glm::vec3(0.0f, 0.0f, 0.0f);
	entity->_collisionMapNumber = 0;

	return 0;
}

int EntityCollision::CalculateEntityCollisions(World* world, Entity* entity)
{
	if (entity->_giveUpAndPushThrough)
		return 1;

	glm::vec3 translation = entity->GetTranslationVector();

	//We don't want to clear the map, because it will throw OutOfRange for each entity again. We also test per entity per frame. So there should never be any collisions
	//world->GetEntityMap()->ClearCheckedByMap();
	EntityArea entityArea = world->GetEntityMap()->GetEntityArea(glm::vec2(translation.x, translation.z), entity->_awareDistance);
	//Loop through every cell in the EntityArea
	for (unsigned int i = 0; i < entityArea.size(); i++)
	{
		//Loop through every entity in the cell
		for (EntityIterator entityItr = entityArea[i]->begin(); entityItr != entityArea[i]->end(); entityItr++)
		{
			//Skip if the entity is ourself
			if ((*entityItr) == entity)
				continue;
			//Skip if the entity is a building
			if ((*entityItr)->GetEntityBase()->EntityTypeInt() == ENTITY_TYPE_BUILDING)
				continue;
			//Skip if we have already checked the entity this frame
			if (world->GetEntityMap()->GetCheckedByMemoryAddress((*entityItr), world->GetFrameNumber()) == entity)
				continue;
			//Otherwise set that we are checking the entity this frame
			else
				world->GetEntityMap()->SetCheckedByMemoryAddress((*entityItr), entity, world->GetFrameNumber());

			glm::vec3 entityTrans = (*entityItr)->GetTranslationVector();

			float distance = sqrt(pow(entityTrans.x - translation.x, 2) + pow(entityTrans.z - translation.z, 2));

			if (distance < entity->GetEntityBase()->MovementCollisionRadius() + (*entityItr)->GetEntityBase()->MovementCollisionRadius())
			{
				float diffDistance = (entity->GetEntityBase()->MovementCollisionRadius() + (*entityItr)->GetEntityBase()->MovementCollisionRadius()) - distance;
				float angleRad = atan2(entityTrans.z - translation.z, entityTrans.x - translation.x);

				entity->_collisionVector.x -= (diffDistance / 2.0f) * glm::cos(angleRad);
				entity->_collisionVector.z -= (diffDistance / 2.0f) * glm::sin(angleRad);
				entity->_collisionNumber++;
			}
		}
	}

	return 0;
}

int EntityCollision::CalculateBuildingCollisions(World* world, Entity* entity)
{
	//TODO: If the entity is in "give up and push through" mode, then this will have potential problems

	glm::vec2 resolvePos;
	glm::vec3 translation = entity->GetTranslationVector();

	//We don't want to clear the map, because it will throw OutOfRange for each entity again. We also test per entity per frame. So there should never be any collisions
	//world->GetEntityMap()->ClearCheckedByMap();
	EntityArea entityArea = world->GetEntityMap()->GetEntityArea(glm::vec2(translation.x, translation.z), entity->_awareDistance);
	//Loop through every cell in the EntityArea
	for (unsigned int i = 0; i < entityArea.size(); i++)
	{
		//Loop through every entity in the cell
		for (EntityIterator entityItr = entityArea[i]->begin(); entityItr != entityArea[i]->end(); entityItr++)
		{
			//Skip if the entity is ourself
			if ((*entityItr) == entity)
				continue;
			//Skip if the entity is NOT a building
			if ((*entityItr)->GetEntityBase()->EntityTypeInt() != ENTITY_TYPE_BUILDING)
				continue;
			//Skip if we have already checked the entity this frame
			if (world->GetEntityMap()->GetCheckedByMemoryAddress((*entityItr), world->GetFrameNumber()) == entity)
				continue;
			//Otherwise set that we are checking the entity this frame
			else
				world->GetEntityMap()->SetCheckedByMemoryAddress((*entityItr), entity, world->GetFrameNumber());

			for (int bbInc = 0; bbInc < (int)(*entityItr)->GetEntityBase()->BoundingBox2D()->Size(); bbInc++)
			{
				BoundingBox2D bb2D = (*entityItr)->GetEntityBase()->BoundingBox2D()->Get(bbInc);
				glm::vec2 p1 = glm::vec2(bb2D.midpoint.x + bb2D.p1Vec.x, bb2D.midpoint.z + bb2D.p1Vec.z) + (*entityItr)->Pos2D();
				glm::vec2 p2 = glm::vec2(bb2D.midpoint.x + bb2D.p4Vec.x, bb2D.midpoint.z + bb2D.p4Vec.z) + (*entityItr)->Pos2D();

				if (RectangleCircleIntersect2D(p1, p2, glm::vec2(bb2D.midpoint.x, bb2D.midpoint.z) + (*entityItr)->Pos2D(),
					(*entityItr)->RotY(), entity->Pos2D(), entity->GetEntityBase()->MovementCollisionRadius(), &resolvePos))
				{
					//DebugOutput("P1: (%f, %f) P2: (%f, %f)\n", p1.x, p1.y, p2.x, p2.y);
					DebugOutput("Inside rect: (%f, %f)\n", resolvePos.x, resolvePos.y);
					entity->SetPosition(resolvePos.x, world->GetPhageTerrain()->CalculateHeight(resolvePos.x, resolvePos.y), resolvePos.y);
				}
			}
		}
	}

	return 0;
}

int EntityCollision::CalculateMapCollisions(World* world, Entity* entity)
{
	float collRad = entity->GetEntityBase()->MovementCollisionRadius();
	glm::vec2 currPos = entity->Pos2D();
	glm::vec2 nextPos = currPos;
	nextPos.x += entity->_collisionVector.x;
	nextPos.y += entity->_collisionVector.z;

	glm::vec2 collPos;
	if (CirlceGridIntersect2D(world, nextPos, collRad, &collPos))
	{
		//Let's first try to see if we can move it away from the first collision
		float boxW = 1.0f / (float)world->GetPathFinder()->GetResolution();
		float boxR = boxW / 2.0f;
		float xDiff = (collPos.x + boxR) - nextPos.x;
		float yDiff = (collPos.y + boxR) - nextPos.y;
		bool hor = glm::abs(xDiff) > glm::abs(yDiff);
		bool neg = hor ? (xDiff < 0.0f) : (yDiff < 0.0f);
		if (hor)
		{
			if (!neg)
				entity->_collisionVector.x = (collPos.x - collRad) - nextPos.x - 0.01f;
			else
				entity->_collisionVector.x = (collPos.x + boxW + collRad) - nextPos.x + 0.01f;
		}
		else
		{
			if (!neg)
				entity->_collisionVector.z = (collPos.y - collRad) - nextPos.y - 0.01f;
			else
				entity->_collisionVector.z = (collPos.y + boxW + collRad) - nextPos.y + 0.01f;
		}
		nextPos.x = currPos.x + entity->_collisionVector.x;
		nextPos.y = currPos.y + entity->_collisionVector.z;
		if (!CirlceGridIntersect2D(world, nextPos, collRad, &collPos))
		{
			entity->_collisionWithMap = true;
			DebugOutput("Shortcut found\n");
			return 1;
		}

		//Unfortunately, an available position has to be found before we can continue. So, we are going to have to search in increasing circles
		float checkDist = 0.0f;
		float checkDistStep = 0.01f;// entity->_moveToNodeVec.x;
		if (checkDistStep < 0.01f) checkDistStep = 0.01f;
		float checkDir = 0.0f;
		float checkDirStep = 0.0;

		checkDist = checkDistStep;
		checkDirStep = 2.0f * glm::asin(checkDistStep / checkDist);
		entity->_collisionVector.x = checkDist * glm::cos(checkDir);
		entity->_collisionVector.z = checkDist * glm::sin(checkDir);
		nextPos.x = currPos.x + entity->_collisionVector.x;
		nextPos.y = currPos.y + entity->_collisionVector.z;

		int checkNum = 0;
		while (CirlceGridIntersect2D(world, nextPos, collRad, &collPos))
		{
			if (checkDir + checkDirStep >= 3.14159265359*2.0f)
			{
				//This is to accelerate the search if we have iterated too many times.
				//TODO: This might need to be changed or a guarenteed bailout might need to be added?
				if (checkNum > 25)
				{
					checkDistStep *= 1.25f;
				}
				checkDist += checkDistStep;
				checkDir = 0.0f;
				checkDirStep = 2.0f * glm::asin(checkDistStep / checkDist);
			}
			else
			{
				checkDir += checkDirStep;
			}
			entity->_collisionVector.x = checkDist * glm::cos(checkDir);
			entity->_collisionVector.z = checkDist * glm::sin(checkDir);
			nextPos.x = currPos.x + entity->_collisionVector.x;
			nextPos.y = currPos.y + entity->_collisionVector.z;
			DebugOutput("Something: %f, %f\n", checkDist, checkDir);
			checkNum++;
		}
		DebugOutput("Resolved\n");
		entity->_collisionWithMap = true;
	}

	return 1;
}

int EntityCollision::HandleCollisions(World* world, Entity* entity)
{
	if (entity->_collisionNumber > 0 && !entity->_collisionWithMap)
	{
		entity->_collisionVector /= (float)entity->_collisionNumber;
		glm::vec3 position = entity->GetTranslationVector();
		glm::vec3 newPosition;
		newPosition.x = position.x + entity->_collisionVector.x;
		newPosition.z = position.z + entity->_collisionVector.z;

		newPosition.y = world->GetPhageTerrain()->CalculateHeight(newPosition.x, newPosition.z);

		entity->SetPosition(newPosition.x, newPosition.y, newPosition.z);
	}
	else if (entity->_collisionWithMap)
	{
		glm::vec3 position = entity->GetTranslationVector();
		glm::vec3 newPosition;
		newPosition.x = position.x + entity->_collisionVector.x;
		newPosition.z = position.z + entity->_collisionVector.z;

		newPosition.y = world->GetPhageTerrain()->CalculateHeight(newPosition.x, newPosition.z);

		entity->SetPosition(newPosition.x, newPosition.y, newPosition.z);
	}
	/*else if (entity->_collisionMapNumber > 0)
	{
		entity->_collisionMapVector /= (float)entity->_collisionMapNumber;
		glm::vec3 position = entity->GetTranslationVector();
		glm::vec3 newPosition;
		newPosition.x = position.x + entity->_collisionMapVector.x;
		newPosition.z = position.z + entity->_collisionMapVector.z;

		newPosition.y = world->GetPhageTerrain()->CalculateHeight(newPosition.x, newPosition.z);

		entity->SetPosition(newPosition.x, newPosition.y, newPosition.z);
	}*/

	return 0;
}

glm::vec2 EntityCollision::RotatePoint2D(glm::vec2 origin, glm::vec2 point, float radians)
{
	glm::vec2 result;
	float c = glm::cos(radians);
	float s = glm::sin(radians);
	result.x = c * (point.x - origin.x) - s * (point.y - origin.y) + origin.x;
	result.y = s * (point.x - origin.x) - c * (point.y - origin.y) + origin.y;

	return result;
}

bool EntityCollision::RectangleCircleIntersect2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 center, float rotation, glm::vec2 circle, float radius, glm::vec2* resolvePosition)
{
	//First rotate the circle center around the rectangle center
	//This will give us the circle center compared to the axis-aligned rectangle
	glm::vec2 rotatedCircle = RotatePoint2D(center, circle, glm::radians(-rotation));

	//Now perform check if circle is intersecting axis-aligned rectangle
	if (rotatedCircle.x + radius < p1.x || rotatedCircle.x - radius > p2.x || rotatedCircle.y + radius < p1.y || rotatedCircle.y - radius > p2.y)
		return false;

	//If we get here, then we are intersecting, so we need to find the position to move the circle
	//Left wall
	float closestDistance = abs(rotatedCircle.x - p1.x);
	resolvePosition->x = p1.x - radius;
	resolvePosition->y = rotatedCircle.y;
	//Right wall
	if (abs(rotatedCircle.x - p2.x) < closestDistance)
	{
		closestDistance = abs(rotatedCircle.x - p2.x);
		resolvePosition->x = p2.x + radius;
		resolvePosition->y = rotatedCircle.y;
	}
	//Top wall
	if (abs(rotatedCircle.y - p1.y) < closestDistance)
	{
		closestDistance = abs(rotatedCircle.y - p1.y);
		resolvePosition->x = rotatedCircle.x;
		resolvePosition->y = p1.y - radius;
	}
	if (abs(rotatedCircle.y - p2.y) < closestDistance)
	{
		closestDistance = abs(rotatedCircle.y - p2.y);
		resolvePosition->x = rotatedCircle.x;
		resolvePosition->y = p2.y + radius;
	}

	(*resolvePosition) = RotatePoint2D(center, (*resolvePosition), glm::radians(rotation));

	return true;
}

bool EntityCollision::CirlceGridIntersect2D(World * world, glm::vec2 center, float radius, glm::vec2* gridPosOut)
{
	float res = (float)world->GetPathFinder()->GetResolution();
	glm::ivec2 boxMin = (center - radius) * res;
	glm::ivec2 boxMax = (center + radius) * res;

	for (int x = boxMin.x; x <= boxMax.x; x++)
	{
		for (int y = boxMin.y; y <= boxMax.y; y++)
		{
			if (world->GetPathFinder()->GetMapValue(x, y) == PathFinder::MAP_CLOSED)
			{
				float fx = (float)x / (float)res;
				float fy = (float)y / (float)res;
				float nearestX = glm::max(fx, glm::min(center.x, fx + 1.0f));
				float nearestY = glm::max(fy, glm::min(center.y, fy + 1.0f));
				if (glm::distance(center, glm::vec2(nearestX, nearestY)) < radius)
				{
					(*gridPosOut).x = (float)x / res;
					(*gridPosOut).y = (float)y / res;
					return true;
				}
			}
		}
	}

	return false;
}
