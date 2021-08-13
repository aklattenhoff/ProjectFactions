#include "EntityMovement.h"

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

EntityMovement::EntityMovement()
{
}

EntityMovement::~EntityMovement()
{
}

int EntityMovement::Init()
{
	return 0;
}

int EntityMovement::StartMovement(World* world, Entity* entity, EntityEvent* evnt)
{
	entity->_hasTargetNode = false;
	entity->_giveUpAndPushThrough = false;
	entity->_previousObstruction = NULL;
	entity->_moveCheckCount = 0;
	entity->_amountToTurn = 0;
	entity->_watchingObstacle = false;

	return 0;
}

int EntityMovement::RequestPath(glm::vec2 pointA, glm::vec2 pointB, World* world, Entity* entity, EntityEvent* evnt)
{
	//If the end point is inaccessible, move it towards the start point until free
	//if (world->GetTerrain()->getTerrainMap()->GetMapPosition(pointB) != MAP_CLEAR)
	if (world->GetPathFinder()->GetMapValueFromPosition(pointB) != PathFinder::MAP_OPEN)
	{
		//float resolution = world->GetTerrain()->getTerrainMap()->GetResolution();
		float resolution = (float)world->GetPathFinder()->GetResolution();
		glm::vec2 stepSize = glm::normalize(pointA - pointB)/resolution;
		//while (world->GetTerrain()->getTerrainMap()->GetMapPosition(pointB) != MAP_CLEAR)
		while (world->GetPathFinder()->GetMapValueFromPosition(pointB) != PathFinder::MAP_OPEN)
			pointB += stepSize;
	}

	//Go ahead and set the desired and current destination. We will update them later to find the acutal destination
	entity->_desiredDestination = pointB;
	entity->_currentDestination = pointB;
	entity->_destinationFlag = false;
	entity->_destinationCheck = 0;
	entity->_destinationCheckDirection = 0.0f;
	entity->_destinationCheckDistance = -1.0f;

	//int pathReturn = world->GetTerrain()->getTerrainMap()->FindPath(pointA, pointB, &evnt->path);
	int pathReturn = world->GetPathFinder()->FindPath(pointA, pointB, &evnt->path);
	if (evnt->path.Size() > 1)
		evnt->path.EraseFront();
	//evnt->path.PushBack(pointB);
	evnt->requestedPath = true;
	evnt->hasPath = true;

	//We need to do some logic here to prevent a large group from having the same node to walk over.
	for (int i = 0; i < (int)evnt->path.Size()-1; i++)
	{
		Path::Node node = evnt->path.At(i);
		int expandNum = 1;
		bool stillSearching = true;
		while (stillSearching && world->GetPathFinder()->GetPathMapVal((int)node.x, (int)node.y) > 5 && expandNum < 5)
		{
			for (int x = -expandNum; stillSearching && x <= expandNum; x++)
			{
				for (int y = -expandNum; stillSearching && y <= expandNum; y++)
				{
					Path::Node tempNode = glm::vec2(node.x + x, node.y + y);
					if (world->GetPathFinder()->GetPathMapVal((int)tempNode.x, (int)tempNode.y) > 5)
						continue;
					if (world->GetPathFinder()->GetMapValueFromPosition(node) != PathFinder::MAP_CLOSED)
					{
						if (i == 0)
						{
							if (world->GetPathFinder()->CheckClearLine(entity->Pos2D(), tempNode, true) == 0)
							{
								if (world->GetPathFinder()->CheckClearLine(tempNode, evnt->path.At(i + 1), true) == 0)
								{
									evnt->path.SetAt(i, tempNode);
									stillSearching = false;
								}
							}
						}
						else
						{
							if (world->GetPathFinder()->CheckClearLine(evnt->path.At(i - 1), tempNode, true) == 0)
							{
								if (world->GetPathFinder()->CheckClearLine(tempNode, evnt->path.At(i + 1), true) == 0)
								{
									evnt->path.SetAt(i, tempNode);
									stillSearching = false;
								}
							}
						}
					}
				}
			}
			node = evnt->path.At(i);
			expandNum++;
		}
	}

	//TODO: I am adding this just to prevent bugs for now
	if (evnt->path.Size() == 0)
	{
		entity->GetEventQueue()->DequeueAndDelete();
		return -1;
	}

	entity->_targetNode = evnt->path.Front();
	entity->_hasTargetNode = true;
	entity->_distanceToTargetNode = glm::distance(entity->Pos2D(), entity->_targetNode);
	entity->_targetNodeCheckValue = 0;

	Path::Node nextNode = evnt->path.Front();
	entity->_moveToNodeVec.x = entity->GetEntityStats()->MovementSpeed()/60.0f;
	entity->_moveToNodeVec.y = glm::degrees(atan2(nextNode.y-entity->PosZ(), nextNode.x-entity->PosX()));
	entity->_moveCurrentVec = entity->_moveToNodeVec;

	entity->_stuckCounter = 0;
	entity->_stuckTime = world->GetGameTime();
	entity->_stuckPos = entity->Pos2D();

	return 0;
}

int EntityMovement::CalculateVectors(World* world, Entity* entity, EntityEvent* evnt)
{
	if (entity->_moveCheckCount > 1)
		return 2;

	//Reset the ToNode vector to zero
	//TODO: Can't this be done in the else instead of everytime? Because we only need it to be zero if there is no movement
	entity->_moveToNodeVec = glm::vec2(0.0f, 0.0f);

	//If there is a node in the current path of the front event in the queue
	if (evnt->hasPath && evnt->path.Size() > 0)
	{
		//Get the next node in the path
		Path::Node nextNode = evnt->path.Front();

		entity->_moveToNodeVec.x = entity->GetEntityStats()->MovementSpeed()/60.0f;
		entity->_moveToNodeVec.y = glm::degrees(atan2(nextNode.y-entity->PosZ(), nextNode.x-entity->PosX()));
		entity->_moveCurrentVec.x = entity->_moveToNodeVec.x;
		entity->_amountToTurn = 0;

		float steerRadius = entity->GetEntityBase()->MovementCollisionRadius();

		entity->_awareDistance = entity->GetEntityBase()->MovementAwareRadius()*2.0f;

		float moveCurrentRadians = glm::radians(entity->_moveCurrentVec.y);
		entity->_steerCurrentVec = glm::vec2(entity->_awareDistance*glm::cos(moveCurrentRadians), entity->_awareDistance*glm::sin(moveCurrentRadians));
		entity->_steerLeftPoint = glm::vec2(steerRadius*cos(moveCurrentRadians+1.57f), steerRadius*sin(moveCurrentRadians+1.57f));
		entity->_steerRightPoint = glm::vec2(steerRadius*cos(moveCurrentRadians-1.57f), steerRadius*sin(moveCurrentRadians-1.57f));
	}

	return 0;
}

int EntityMovement::CalculateMovement(World* world, Entity* entity, EntityEvent* evnt)
{
	if (entity->_moveCheckCount > 1)
		return 2;

	glm::vec3 translation = entity->GetTranslationVector();
	float distanceToGoal = glm::distance(entity->Pos2D(), evnt->path.Back());
	float distanceToNode = glm::distance(entity->Pos2D(), evnt->path.Front());

	//Calculate the entities in the area around the entity
	//NOTE: The EntityArea computed by this function will contain duplicates which need to be filtered
	//out by the checkedBy memory address in the EntityMap
	EntityArea entityArea = world->GetEntityMap()->GetEntityArea(glm::vec2(translation.x, translation.z), entity->_awareDistance);

	float obstructionDistance = -1.0f;
	float obstructionAngleDeg = 0.0f;
	Entity* obstruction = NULL;
	//Loop through every cell in the EntityArea
	for (unsigned int i = 0;i < entityArea.size();i ++)
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

			float distance = sqrt(pow(entityTrans.x-translation.x, 2) + pow(entityTrans.z-translation.z, 2));
			float angleRad = atan2(entityTrans.z-translation.z, entityTrans.x-translation.x);
			float angleDeg = glm::degrees(angleRad);
			float angleBetweenVecsDeg = abs(angleDeg - entity->_moveCurrentVec.y);
			float distToCurrentVec = distance * sin(glm::radians(angleBetweenVecsDeg));
			float checkDist = 0.8f;
			float checkRad = 0.4f;

			checkDist = entity->_awareDistance + (*entityItr)->GetEntityBase()->MovementAwareRadius();
			checkRad = (*entityItr)->GetEntityBase()->MovementAwareRadius();

			if (angleBetweenVecsDeg < 90.0f && //If the obstruction is in front of us
				distanceToGoal > distance && //If the obstruction is closer than the end of the path
				distance < checkDist && //If the obstruction is closer than our steer cylinder
				(LineCircleIntersect2D(entity->Pos2D(), entity->_moveCurrentVec, (*entityItr)->Pos2D(), checkRad) ||
				LineCircleIntersect2D(entity->Pos2D()+entity->_steerLeftPoint, entity->_moveCurrentVec, (*entityItr)->Pos2D(), checkRad) ||
				LineCircleIntersect2D(entity->Pos2D()+entity->_steerRightPoint, entity->_moveCurrentVec, (*entityItr)->Pos2D(), checkRad)))
				//distToCurrentVec < entity->GetEntityStats()->AwarenessRadius() + (*entityItr)->GetEntityStats()->AwarenessRadius())
			{
				if (obstructionDistance < 0.0f || distance < obstructionDistance)
				{
					obstructionDistance = distance;
					obstructionAngleDeg = angleDeg;
					obstruction = (*entityItr);
				}
			}
		}
	}

	//This is the give up and push through logic
	float dirDiff = entity->_moveCurrentVec.y - entity->_moveToNodeVec.y;
	//dirDiff = std::fmod(dirDiff+180.0f, 360.0f) - 180.0f;
	if (dirDiff > 80.0f || dirDiff < -80.0f)
	{
		entity->_giveUpAndPushThrough = true;
	}

	if (distanceToNode < entity->_awareDistance)
	{
		entity->_moveCurrentVec.y = entity->_moveToNodeVec.y;
	}

	if (entity->_giveUpAndPushThrough)
	{
		if (!obstruction) //we have pushed through and can stop
			entity->_giveUpAndPushThrough = false;
		else //continue to push through
		{
			obstruction = NULL;
			entity->_moveCurrentVec.x *= 0.5f;
		}
	}

	/*if (entity->_oscillationPrevent != 0)
	{
		if (entity->_oscillationPrevent > 0)
			entity->_oscillationPrevent--;
		else
			entity->_oscillationPrevent++;
	}*/

	//Found the closest obstruction
	if (obstruction)
	{
		glm::vec2 obstPos = glm::vec2(obstruction->PosX(), obstruction->PosZ());

		//Project obstruction's movement
		if (obstruction->_moveCurrentVec.x > 0.0f)
		{
			float t = 0.5f*obstructionDistance / (entity->GetEntityStats()->MovementSpeed()/60.0f);
			obstPos.x += t * obstruction->_moveCurrentVec.x * cos(glm::radians(obstruction->_moveCurrentVec.y));
			obstPos.y += t * obstruction->_moveCurrentVec.x * sin(glm::radians(obstruction->_moveCurrentVec.y));
		}

		float angleDeg = glm::degrees(atan2(obstPos.y-translation.z, obstPos.x-translation.x));

		float dirToTurn = entity->_moveCurrentVec.y - angleDeg;
		bool positive = dirToTurn > 0.0f;
		
		//Do we need to turn?
		if (abs(angleDeg - entity->_moveToNodeVec.y) < 60.0f)
		{
			if (positive)// && entity->_oscillationPrevent >= 0)
			{
				entity->_amountToTurn = (int)entity->GetEntityBase()->MovementTurnRate();
				entity->_oscillationPrevent = 2;
			}
			else// if (!positive && entity->_oscillationPrevent <= 0)
			{
				entity->_amountToTurn = (int)-entity->GetEntityBase()->MovementTurnRate();
				entity->_oscillationPrevent = -2;
			}
			entity->_moveCurrentVec.x *= 0.75f;

			entity->_previousObstruction = obstruction;
		}

		//If we are too close to an entity moving the same direction, just stop and wait for him to gain distance
		if (obstruction->_moveCurrentVec.x > 0.0f && abs(obstruction->_moveCurrentVec.y - entity->_moveCurrentVec.y) < 25.0f)
		{
			entity->_stopAndWaitCounter = 5;
		}
	}
	if (entity->_stopAndWaitCounter > 0)
	{
		entity->_stopAndWaitCounter--;
		entity->_moveCurrentVec.x *= 0.0f;
		if (entity->GetModel()->GetAnimation(Model::ANIMATION_IDLE_1) && entity->IsAlive())
			entity->SetAnimationIndex(Model::ANIMATION_IDLE_1);
		//entity->SetEntityStatus(Entity::STATUS_IDLING);
	}
	else
	{
		//entity->SetEntityStatus(Entity::STATUS_WALKING);
		if (entity->GetModel()->GetAnimation(Model::ANIMATION_WALKING) && entity->IsAlive())
			entity->SetAnimationIndex(Model::ANIMATION_WALKING);
	}
	if (!obstruction)
	{
		float turnRate = entity->GetEntityBase()->MovementTurnRate();
		float dirToTurn = entity->_moveCurrentVec.y - entity->_moveToNodeVec.y;
		while (dirToTurn > 180.0f) dirToTurn -= 360.0f;
		while (dirToTurn < -180.0f) dirToTurn += 360.0f;
		glm::vec2 previousMoveCurrVec = entity->_moveCurrentVec;
		if (dirToTurn > -turnRate && dirToTurn < turnRate)
		{
			entity->_moveCurrentVec.y = entity->_moveToNodeVec.y;
		}
		else
		{
			bool positive = dirToTurn > 0.0f;
			if (!positive)// && entity->_oscillationPrevent >= 0)
			{
				entity->_amountToTurn = (int)turnRate;
				entity->_oscillationPrevent = 2;
			}
			else// if (positive && entity->_oscillationPrevent <= 0)
			{
				entity->_amountToTurn = (int)-turnRate;
				entity->_oscillationPrevent = -2;
			}
			entity->_moveCurrentVec.x *= 0.75f;
		}
		if (entity->_previousObstruction)
		{
			glm::vec3 obstTrans = entity->_previousObstruction->GetTranslationVector();
			float distance = sqrt(pow(obstTrans.x - translation.x, 2) + pow(obstTrans.z - translation.z, 2));
			float angleRad = atan2(obstTrans.z - translation.z, obstTrans.x - translation.x);
			float angleDeg = glm::degrees(angleRad);
			glm::vec2 newMoveCurrVec = glm::vec2(entity->_moveCurrentVec.x, entity->_moveCurrentVec.y + entity->_amountToTurn);
			float angleBetweenVecsDeg = abs(angleDeg - newMoveCurrVec.y);
			float distToCurrentVec = distance * sin(glm::radians(angleBetweenVecsDeg));
			float checkDist = 0.8f;
			float checkRad = 0.4f;

			checkDist = entity->_awareDistance + entity->_previousObstruction->GetEntityBase()->MovementAwareRadius();
			checkRad = entity->_previousObstruction->GetEntityBase()->MovementAwareRadius();

			if (angleBetweenVecsDeg < 90.0f &&
				distance < checkDist &&
				(LineCircleIntersect2D(entity->Pos2D(), newMoveCurrVec, entity->_previousObstruction->Pos2D(), checkRad) ||
					LineCircleIntersect2D(entity->Pos2D() + entity->_steerLeftPoint, newMoveCurrVec, entity->_previousObstruction->Pos2D(), checkRad) ||
					LineCircleIntersect2D(entity->Pos2D() + entity->_steerRightPoint, newMoveCurrVec, entity->_previousObstruction->Pos2D(), checkRad)))
			{
				entity->_amountToTurn = 0;
			}
		}
	}

	//Adjust movement and turning speed
	float absDiff = glm::abs(dirDiff);
	float diffRatio = absDiff / 180.0f;
	float diffVal = glm::clamp((diffRatio - 0.5f) * 2.0f, 0.0f, 1.0f);
	if (absDiff > 90.0f && absDiff < 179.0f)
		entity->_moveCurrentVec.x *= (1.0f - diffVal);

	if (obstruction)
		entity->_currentObstruction = obstruction;
	else
		entity->_currentObstruction = NULL;

	if (entity->_watchingObstacle)
	{
		float collRad = entity->GetEntityBase()->MovementCollisionRadius();
		glm::vec2 currPos = entity->Pos2D();
		glm::vec2 newPos;
		newPos.x = currPos.x + entity->_moveCurrentVec.x*cos(glm::radians(entity->_obstacleDirection));
		newPos.y = currPos.y + entity->_moveCurrentVec.x*sin(glm::radians(entity->_obstacleDirection));
		if (!CirlceGridIntersect2D(world, newPos, collRad))
		{
			entity->_watchingObstacle = false;
		}
		else
		{
			entity->_amountToTurn = 0;
		}
	}
	//Next we need to determine ObstacleMap collisions
	{
		float collRad = entity->GetEntityBase()->MovementCollisionRadius();
		glm::vec2 currPos = entity->Pos2D();
		glm::vec2 newPos;
		newPos.x = currPos.x + entity->_moveCurrentVec.x*cos(glm::radians(entity->_moveCurrentVec.y));
		newPos.y = currPos.y + entity->_moveCurrentVec.x*sin(glm::radians(entity->_moveCurrentVec.y));
		if (CirlceGridIntersect2D(world, newPos, collRad))
		{
			glm::vec2 xPos = currPos;
			xPos.x += cos(glm::radians(entity->_moveCurrentVec.y)) < 0.0f ? -entity->_moveCurrentVec.x : entity->_moveCurrentVec.x;
			glm::vec2 yPos = currPos;
			yPos.y += sin(glm::radians(entity->_moveCurrentVec.y)) < 0.0f ? -entity->_moveCurrentVec.x : entity->_moveCurrentVec.x;
			if (!CirlceGridIntersect2D(world, xPos, collRad))
			{
				entity->_watchingObstacle = true;
				entity->_obstacleDirection = entity->_moveCurrentVec.y;
				entity->_moveCurrentVec.y = cos(glm::radians(entity->_moveToNodeVec.y)) < 0.0f ? 180.0f : 0.0f;
				entity->_amountToTurn = 0;
			}
			else if (!CirlceGridIntersect2D(world, yPos, collRad))
			{
				entity->_watchingObstacle = true;
				entity->_obstacleDirection = entity->_moveCurrentVec.y;
				entity->_moveCurrentVec.y = sin(glm::radians(entity->_moveToNodeVec.y)) < 0.0f ? 270.0f : 90.0f;
				entity->_amountToTurn = 0;
			}
			else
			{
				DebugOutput("ASDASDASDASDhdfghkjhsaf\n");
				/*entity->_moveCurrentVec.x = 0.0f;
				entity->GetEventQueue()->DequeueAndDelete();
				if (entity->GetModel()->GetAnimation(Model::ANIMATION_IDLE_1))
					entity->SetAnimationIndex(Model::ANIMATION_IDLE_1);
				return -1;*/
			}
			//entity->_moveCurrentVec.y = cos(glm::radians(entity->_moveCurrentVec.y)) < 0.0f ? 180.0f : 0.0f;
			//entity->_amountToTurn = 0.0f;
		}
	}

	float changeInDistance = entity->_distanceToTargetNode - distanceToNode;
	entity->_distanceToTargetNode = distanceToNode;
	if (changeInDistance < 0.0f)
	{
		if (evnt->path.Size() > 1 && world->GetPathFinder()->CheckClearLine(entity->Pos2D(), evnt->path.At(1), true) == 0)
		{
			DebugOutput("Bipassing current node\n");
			evnt->path.EraseFront();
		}
		else if (world->GetPathFinder()->CheckClearLine(entity->Pos2D(), evnt->path.Front(), true) == 1)
		{
			EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();
			evnt->initialized = false;
			evnt->requestedPath = false;
			evnt->path.Clear();
			DebugOutput("Lost clear line to node - resetting path\n");
		}
		else
		{
			entity->_moveCurrentVec.x = 0.0f;
			DebugOutput("Going backwards - stopping\n");
		}
	}

	//Now let's make sure the destination isn't blocked by other entities if we haven't already.
	//The idea is that once an entity gets access to a free destination, no other entity can claim that destination.
	//This may change if we want to add priority based on closeness to the destination
	while (!entity->_destinationFlag)
	{
		if (CalculateDestination(world, entity, evnt) == 0)
			entity->_destinationFlag = true;
		else
		{
			//DebugOutput("COLLISION!!!");
		}
	}

	return 0;
}

int EntityMovement::MoveEntity(World* world, Entity* entity)
{
	entity->_moveCheckCount --;
	if (entity->_moveCheckCount <= 0)
		entity->_moveCheckCount = 3;

	entity->_moveCurrentVec.y += entity->_amountToTurn / 3.0f;

	//Change direction of the entity
	//NOTE: This needs adjustement if zero degrees isn't in the +X direction
	entity->SetRotation(-entity->_moveCurrentVec.y+90.0f);

	glm::vec3 tv = entity->GetTranslationVector();
	glm::vec2 mcv = entity->_moveCurrentVec;
	glm::vec2 scv = entity->_steerCurrentVec;
	glm::vec2 slp = glm::vec2(tv.x, tv.z)+entity->_steerLeftPoint;
	glm::vec2 srp = glm::vec2(tv.x, tv.z)+entity->_steerRightPoint;
	glm::vec2 sv = glm::vec2(tv.x+scv.x, tv.z+scv.y);
	glm::vec2 slv = glm::vec2(slp.x+scv.x, slp.y+scv.y);
	glm::vec2 srv = glm::vec2(srp.x+scv.x, srp.y+scv.y);
	//RendererSingleton->AddLineDrawInfo(tv, glm::vec3(sv.x, tv.y, sv.y));
	//RendererSingleton->AddLineDrawInfo(glm::vec3(slp.x, tv.y, slp.y), glm::vec3(slv.x, tv.y, slv.y));
	//RendererSingleton->AddLineDrawInfo(glm::vec3(srp.x, tv.y, srp.y), glm::vec3(srv.x, tv.y, srv.y));

	if (entity->_giveUpAndPushThrough)
	{
		glm::vec3 tr = entity->GetTranslationVector();
		//RendererSingleton->AddLineDrawInfo(glm::vec3(tr.x-0.25f, tr.y, tr.z-0.25f), glm::vec3(tr.x+0.25f, tr.y, tr.z+0.25f));
		//RendererSingleton->AddLineDrawInfo(glm::vec3(tr.x+0.25f, tr.y, tr.z-0.25f), glm::vec3(tr.x-0.25f, tr.y, tr.z+0.25f));
	}

	if (entity->_currentObstruction)
	{
		glm::vec3 op = entity->_currentObstruction->GetTranslationVector();
		float sqSize = 0.25f;
		//RendererSingleton->AddLineDrawInfo(glm::vec3(op.x-sqSize, op.y, op.z-sqSize), glm::vec3(op.x+sqSize, op.y, op.z-sqSize));
		//RendererSingleton->AddLineDrawInfo(glm::vec3(op.x+sqSize, op.y, op.z-sqSize), glm::vec3(op.x+sqSize, op.y, op.z+sqSize));
		//RendererSingleton->AddLineDrawInfo(glm::vec3(op.x+sqSize, op.y, op.z+sqSize), glm::vec3(op.x-sqSize, op.y, op.z+sqSize));
		//RendererSingleton->AddLineDrawInfo(glm::vec3(op.x-sqSize, op.y, op.z+sqSize), glm::vec3(op.x-sqSize, op.y, op.z-sqSize));
	}
	
	//Change the position of the entity based on the movement vector
	glm::vec3 position = entity->GetTranslationVector();
	//EuclideanVector2D moveVec = entity->GetMovementVector();
	glm::vec3 newPosition;
	//newPosition.x = position.x + moveVec.magnitude*cos(glm::radians(moveVec.direction));
	//newPosition.z = position.z + moveVec.magnitude*sin(glm::radians(moveVec.direction));
	newPosition.x = position.x + entity->_moveCurrentVec.x*cos(glm::radians(entity->_moveCurrentVec.y));
	newPosition.z = position.z + entity->_moveCurrentVec.x*sin(glm::radians(entity->_moveCurrentVec.y));

	//Recalculate the terrain height at the new position
	newPosition.y = world->GetPhageTerrain()->CalculateHeight(newPosition.x, newPosition.z);//world->GetTerrain()->getInterpolatedHeight(newPosition.x, newPosition.z);

	//Save previous position if we are actually stuppose to move
	unsigned int gameTime = world->GetGameTime();
	if (gameTime - entity->_stuckTime > 1000)
	{
		float length = glm::distance(entity->_stuckPos, entity->Pos2D());
		//DebugOutput("Window movement: %f\n", length);
		entity->_stuckTime = gameTime;
		entity->_stuckPos = entity->Pos2D();

		if (length < 0.1f)
		{
			EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();
			evnt->initialized = false;
			evnt->requestedPath = false;
			evnt->path.Clear();
			DebugOutput("Stuck - resetting path\n");
		}
	}

	//Actually move the entity
	entity->SetPosition(newPosition.x, newPosition.y, newPosition.z);

	//TODO: Should this be moved somewhere else?
	world->GetEntityMap()->CalcMovement(entity);

	//Debug stuff
	//RendererSingleton->AddLineDrawInfo(position, position + glm::vec3(scv.x, 0.0f, scv.y));
	Path p = entity->GetEventQueue()->GetFrontEvent()->path;
	p.DetachPathFinder();
	for (int i = 0; i < (int)p.Size(); i++)
	{
		float h = world->GetPhageTerrain()->CalculateHeight(p.At(i).x, p.At(i).y);
		RendererSingleton->AddLineDrawInfo(glm::vec3(p.At(i).x - 0.25f, h, p.At(i).y - 0.25f), glm::vec3(p.At(i).x + 0.25f, h, p.At(i).y + 0.25f));
		RendererSingleton->AddLineDrawInfo(glm::vec3(p.At(i).x - 0.25f, h, p.At(i).y + 0.25f), glm::vec3(p.At(i).x + 0.25f, h, p.At(i).y - 0.25f));
	}
	for (int i = 0; i < (int)p.Size(); i++)
	{
		if (i == 0)
		{
			float h = world->GetPhageTerrain()->CalculateHeight(p.Front().x, p.Front().y);
			//RendererSingleton->AddLineDrawInfo(entity->GetTranslationVector(), glm::vec3(p.Front().x, h, p.Front().y));
		}
		else
		{
			float h1 = world->GetPhageTerrain()->CalculateHeight(p.At(i-1).x, p.At(i-1).y);
			float h2 = world->GetPhageTerrain()->CalculateHeight(p.At(i).x, p.At(i).y);
			//RendererSingleton->AddLineDrawInfo(glm::vec3(p.At(i-1).x, h1, p.At(i-1).y), glm::vec3(p.At(i).x, h2, p.At(i).y));
		}

	}

	return 0;
}

bool EntityMovement::LineCircleIntersect2D(glm::vec2 pt1, glm::vec2 magAndDir, glm::vec2 circle, float radius)
{
	//Tangent is the magAndDir of the line
	//Hypotenuse is from line start to circle center
	//Angle is the angle between Tangent and Hypotenuse
	float distanceHypotenuse = glm::distance(pt1, circle);
	float directionHypotenuse = atan2(circle.y-pt1.y, circle.x-pt1.x);
	float angle = abs(directionHypotenuse - glm::radians(magAndDir.y));
	float oppositeMag = distanceHypotenuse * glm::sin(angle);

	return (oppositeMag <= radius);
}

glm::vec2 EntityMovement::RotatePoint2D(glm::vec2 origin, glm::vec2 point, float radians)
{
	glm::vec2 result;
	float c = glm::cos(radians);
	float s = glm::sin(radians);
	result.x = c * (point.x - origin.x) - s * (point.y - origin.y) + origin.x;
	result.y = s * (point.x - origin.x) - c * (point.y - origin.y) + origin.y;

	return result;
}

bool EntityMovement::RectangleCircleIntersect2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 center, float rotation, glm::vec2 circle, float radius)
{
	//First rotate the circle center around the rectangle center
	//This will give us the circle center compared to the axis-aligned rectangle
	glm::vec2 rotatedCircle = RotatePoint2D(center, circle, glm::radians(-rotation));

	//Now perform check if circle is intersecting axis-aligned rectangle
	if (circle.x + radius < p1.x || circle.x - radius > p2.x || circle.y + radius < p1.y || circle.y - radius > p2.y)
		return false;

	return true;
}

int EntityMovement::CalculateDestination(World* world, Entity* entity, EntityEvent* evnt)
{
	//NOTE: We can reuse the checkedBy Map but we need to clear it first
	//We don't want to clear the map, because it will throw OutOfRange for each entity again. We also test per entity per frame. So there should never be any collisions
	//world->GetEntityMap()->ClearCheckedByMap();

	//Calculate the entity destinations in the area around the entity's destination
	EntityArea entityArea = world->GetEntityMap()->GetEntityDestinationArea(entity->_currentDestination, entity->_awareDistance);

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

			float distance = sqrt(pow((*entityItr)->_currentDestination.x - entity->_currentDestination.x, 2) +
									pow((*entityItr)->_currentDestination.y - entity->_currentDestination.y, 2));
			float checkDist = entity->GetEntityBase()->MovementAwareRadius() + (*entityItr)->GetEntityBase()->MovementAwareRadius();

			if (distance < checkDist)
			{
				//The desired destination is not available, so we need to find somewhere else.
				//NOTE: For now, let's set the currentDestination so that a new spot is checked next frame.
				//TODO: We may need to change this if more than one spot a frame needs to be checked
				//float direction = atan2(entity->_desiredDestination.y - entity->PosZ(), entity->_desiredDestination.x - entity->PosX());
				//entity->_currentDestination.x -= checkDist * glm::cos(direction);
				//entity->_currentDestination.y -= checkDist * glm::sin(direction);

				if (entity->_destinationCheckDistance <= 0.0f || (entity->_destinationCheckDirection + entity->_destinationCheckDirectionStep >= 3.14159265359*2.0f))
				{
					if (entity->_destinationCheckDistance <= 0.0f)
						entity->_destinationCheckDistance = checkDist;
					else
						entity->_destinationCheckDistance += (*entityItr)->GetEntityBase()->MovementAwareRadius() * 2.0f;

					entity->_destinationCheckDirection = 0.0f;
					entity->_destinationCheckDirectionStep = 2.0f * glm::asin(entity->GetEntityBase()->MovementAwareRadius() / entity->_destinationCheckDistance);
				}
				else
				{
					entity->_destinationCheckDirection += entity->_destinationCheckDirectionStep;
				}
				entity->_currentDestination.x = entity->_desiredDestination.x + entity->_destinationCheckDistance * glm::cos(entity->_destinationCheckDirection);
				entity->_currentDestination.y = entity->_desiredDestination.y + entity->_destinationCheckDistance * glm::sin(entity->_destinationCheckDirection);


				return -1;
			}
		}
	}

	//If we got out of this loop, then the desired destination is available. So, let's claim it.
	world->GetEntityMap()->RemoveDestination(entity);
	world->GetEntityMap()->AddDestination(entity, entity->_currentDestination);
	evnt->path.SetAt(evnt->path.Size() - 1, entity->_currentDestination);

	return 0;
}

bool EntityMovement::CirlceGridIntersect2D(World * world, glm::vec2 center, float radius)
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
				//some additional grid-circle intersection
				{
					return true;
				}
			}
		}
	}

	return false;
}

int EntityMovement::CleanUp(World* world, Entity* entity)
{
	//Get the event queue for the entity so that we can check if there is a path to follow
	EntityEventQueue* entityEventQueue = entity->GetEventQueue();

	//If there is a node in the current path of the front event in the queue
	if (entityEventQueue->Size() > 0 && entityEventQueue->GetFrontEvent()->path.Size() > 0)
	{
		//Get the next node in the path
		Path::Node nextNode = entityEventQueue->GetFrontEvent()->path.Front();
		//If we are close enough to the path node
		float distanceToNode = sqrt(pow(nextNode.x - entity->PosX(), 2) + pow(nextNode.y - entity->PosZ(), 2));
		if ((entityEventQueue->GetFrontEvent()->path.Size() > 1 && distanceToNode < entity->GetEntityBase()->MovementCollisionRadius() || distanceToNode < entity->_moveCurrentVec.x) && entity->_destinationFlag)
		//if (distanceToNode < /*entity->GetMovementVector().magnitude*/entity->GetEntityBase()->MovementCollisionRadius() && entity->_destinationFlag)
		{
			if (entityEventQueue->GetFrontEvent()->path.Size() == 1)
				entity->SetPosition(nextNode.x, world->GetPhageTerrain()->CalculateHeight(nextNode.x, nextNode.y), nextNode.y);
			entityEventQueue->GetFrontEvent()->path.EraseFront();
			//entity->ResetMovementVector();
			//entity->_moveCurrentVec = glm::vec2(0.0f, 0.0f);

			if (entityEventQueue->GetFrontEvent()->path.Size() <= 0)
			{
				if (entityEventQueue->GetFrontEvent()->eventType == EntityEvent::MOVE_TO_POSITION ||
					entityEventQueue->GetFrontEvent()->eventType == EntityEvent::MOVE_TO_INSTANCE)
				{
					entity->_moveCurrentVec = glm::vec2(0.0f, 0.0f);
					entityEventQueue->DequeueAndDelete();
					entity->SetEntityStatus(Entity::STATUS_IDLING);
				}
			}
		}
	}

	return 0;
}

int EntityMovement::StopMovement(World* world, Entity* entity)
{
	entity->_moveCurrentVec = glm::vec2(0.0f, 0.0f);

	return 0;
}
