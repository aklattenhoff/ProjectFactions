#include "LMGatherResource.h"

#include "Game/LogicModules/LMMovement.h"

#include "Game/Entity/EntityEventQueue.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityBase.h"

#include "Game/EntityMovement/EntityMovement.h"

//#include "Game/Terrain/Terrain.h"
//#include "Game/Terrain/TerrainMap.h"

#include "Game/Math/BoundingBox2DSet.h"

#include "Game/Player/PlayerList.h"
#include "Game/Player/Player.h"

int LMGatherResource::Process(World* world, Entity* entity, int phase)
{
	switch (phase)
	{
	case 0:
		CheckDistance(world, entity, phase);
		break;
	case 1:
		CheckDistance(world, entity, phase);
		GatherResource(world, entity, phase);
		break;
	case 2:
		CheckDistance(world, entity, phase);
		break;
	}

	return 0;
}

int GetAnimationType(std::string str)
{
	int returnInt = -1;

	if (str.compare("Food") == 0)
		returnInt = Model::ANIMATION_GATHERING_FOOD;
	else if (str.compare("Wood") == 0)
		returnInt = Model::ANIMATION_GATHERING_WOOD;
	else if (str.compare("Stone") == 0)
		returnInt = Model::ANIMATION_GATHERING_STONE;
	else if (str.compare("Alloy") == 0)
		returnInt = Model::ANIMATION_GATHERING_ALLOY;
	else if (str.compare("Huntable") == 0)
		returnInt = Model::ANIMATION_GATHERING_HUNTABLE;
	else if (str.compare("Herdable") == 0)
		returnInt = Model::ANIMATION_GATHERING_HERDABLE;
	else if (str.compare("Farm") == 0)
		returnInt = Model::ANIMATION_GATHERING_FARM;
	else if (str.compare("Tree") == 0)
		returnInt = Model::ANIMATION_GATHERING_TREE;

	return returnInt;
}

int LMGatherResource::GatherResource(World* world, Entity* entity, int phase)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (!evnt->atGatherArea && evnt->needToMove)
		return 0;
	
	if (!evnt->hasGatherArea)
		entity->SetRotation(-glm::degrees(atan2(evnt->entity->PosZ()-entity->PosZ(), evnt->entity->PosX()-entity->PosX()))+90.0f);

	//Gather resource
	if (!evnt->foundGatherType)
	{
		//These values deal with the animation for the gathering
		entity->SetGatherType(-1);
		entity->SetGatherOverwriteType(-1);

		EntityXMLTag::GatherRateSet* gatherRateSet = &entity->GetEntityBase()->GetEntityXML()->gatherRateSet;
		for (int i = 0;i < (int)gatherRateSet->size();i ++)
		{
			if (evnt->entity->GetEntityBase()->ContainsClass(gatherRateSet->at(i).type))
			{
				DebugOutput("Contains Gather: %s\n", gatherRateSet->at(i).type.c_str());
				entity->SetGatherType(GetAnimationType(gatherRateSet->at(i).type));
				evnt->gatherRate = gatherRateSet->at(i).value;
			}
		}
		EntityXMLTag::GatherRateSet* gatherRateOverwriteSet = &entity->GetEntityBase()->GetEntityXML()->gatherRateOverwriteSet;
		for (int i = 0;i < (int)gatherRateOverwriteSet->size();i ++)
		{
			if (evnt->entity->GetEntityBase()->ContainsClass(gatherRateOverwriteSet->at(i).type))
			{
				DebugOutput("Contains Gather Over: %s\n", gatherRateOverwriteSet->at(i).type.c_str());
				entity->SetGatherOverwriteType(GetAnimationType(gatherRateOverwriteSet->at(i).type));
				evnt->gatherRate = gatherRateOverwriteSet->at(i).value;
			}
		}

		evnt->foundGatherType = true;
	}

	//Gather timer info
	if (evnt->gatherTimer < 0)
	{
		evnt->gatherTimer = SDL_GetTicks();
	}
	else
	{
		int gatherTime = SDL_GetTicks() - evnt->gatherTimer;
		evnt->gatherTimer = SDL_GetTicks();
		float gatherSecs = ((float)gatherTime)/1000.0f;
		float gatherValue = gatherSecs * evnt->gatherRate;

		std::vector<EntityXMLTag::Resource> resourceVec =  evnt->entity->GetEntityBase()->GetEntityXML()->resourceVector;
		for (int i = 0;i < (int)resourceVec.size();i ++)
		{
			int resourceType = Player::GetResourceIndex(resourceVec[i].type);
			world->GetPlayerList()->GetAt(entity->GetOwningPlayer())->ModifyResource(resourceType, gatherValue);
		}
	}

	return 0;
}

int LMGatherResource::CheckDistance(World* world, Entity* entity, int phase)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();
	
	if (evnt->atGatherArea && (evnt->hasGatherArea || evnt->entity->GetEntityBase()->GatherArea()->Size() > 0))
	{
		evnt->hasGatherArea = true;
		if (SDL_GetTicks() - evnt->frameCount > 9000)
		{
			BoundingBox2DSet* gatherArea = evnt->entity->GetEntityBase()->GatherArea();
			int areaVal = rand() % gatherArea->Size();
			glm::vec2 areaSize = glm::vec2(gatherArea->Get(areaVal).point2.x - gatherArea->Get(areaVal).point1.x, gatherArea->Get(areaVal).width);
			glm::vec3 randPoint = glm::vec3(0.0f);
			randPoint.x = gatherArea->Get(areaVal).point1.x + fmod((float)rand(), areaSize.x*100.0f)/100.0f;
			randPoint.z = (gatherArea->Get(areaVal).point1.z - gatherArea->Get(areaVal).width/2.0f) + fmod((float)rand(), areaSize.y*100.0f)/100.0f;
			glm::vec3 finalRandPoint = glm::rotate(randPoint, glm::radians(evnt->entity->RotY()), glm::vec3(0.0f, 1.0f, 0.0f));
			finalRandPoint += evnt->entity->GetTranslationVector();
			evnt->moveToPosition = glm::vec2(finalRandPoint.x, finalRandPoint.z);
			evnt->frameCount = SDL_GetTicks();

			evnt->path.Clear();
			evnt->initialized = false;
			evnt->requestedPath = false;
			evnt->hasPath = false;
		}
	}
	else if (!evnt->atGatherArea)
	{
		if (!evnt->hasSetGatherRallyPoint)
		{
			if (evnt->entity->GetEntityBase()->GatherArea()->Size() > 0)
			{
				BoundingBox2D rallyBox = evnt->entity->GetEntityBase()->GatherArea()->Get(0);
				glm::vec3 rotVal = glm::rotate(rallyBox.midpoint, glm::radians(evnt->entity->RotY()), glm::vec3(0.0f, 1.0f, 0.0f));
				evnt->moveToPosition = evnt->entity->Pos2D() + glm::vec2(rotVal.x, rotVal.z);
			}
			else
			{
				evnt->moveToPosition = evnt->entity->Pos2D();
			}
			evnt->hasSetGatherRallyPoint = true;
		}
	}

	if (phase == 0)
	{
		//Calculate the distance
		//glm::vec3 midP = evnt->entity->GetTranslationVector() + evnt->entity->GetEntityBase()->GatherArea()->Get(0).midpoint;
		float distance = sqrt(pow(evnt->moveToPosition.x - entity->PosX(), 2.0f)+pow(evnt->moveToPosition.y - entity->PosZ(), 2.0f));

		//Set whether movement is needed for this event
		float minGatherDist = 1.5f;
		if (distance > minGatherDist/*ADDVAR: Min distance to gather*/)
			evnt->needToMove = true;
		else
		{
			evnt->atGatherArea = true;
			evnt->needToMove = false;
		}

		//If the given destination is too far away from the gather point, we need to change it
		if (evnt->needToMove && evnt->path.Size() > 0 && glm::distance(evnt->path.Back(), evnt->moveToPosition) > minGatherDist)
		{
			glm::vec2 displaceVec = glm::normalize(evnt->path.Back() - evnt->moveToPosition) * minGatherDist;
			evnt->path.SetAt(evnt->path.Size() - 1, evnt->moveToPosition + displaceVec);
		}

		//If we need to move and there is no path, or the target entity has moved off target, then create a new path.
		if (evnt->needToMove && evnt->path.Size() <= 0)
		{
			evnt->path.Clear();
			evnt->initialized = false;
			evnt->requestedPath = false;
		}
	}

	//If there is movement is needed, then process movement at each phase
	if (evnt->needToMove)
	{
		entity->SetEntityStatus(Entity::STATUS_WALKING);
		LMMovement::ProcessMovement(world, entity, phase);
	}
	else
	{
		world->GetEntityMovement()->StopMovement(world, entity);
	}

	return 0;
}
