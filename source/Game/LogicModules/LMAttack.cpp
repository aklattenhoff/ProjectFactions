#include "LMAttack.h"

#include "Game/LogicModules/LMMovement.h"

#include "Game/Entity/EntityEventQueue.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityBase.h"
#include "Phage/Backend/Animation.h"

//#include "Game/Terrain/Terrain.h"
//#include "Game/Terrain/TerrainMap.h"

#include "Game/ProjectileSystem/ProjectileSystem.h"
#include "Game/ProjectileSystem/ProjectileBase.h"
#include "Game/ProjectileSystem/Projectile.h"
#include "Game/Entity/EntityBaseList.h"

int LMAttack::Process(World* world, Entity* entity, int phase)
{
	switch (phase)
	{
	case 0:
		CheckDistance(world, entity, phase);
		break;
	case 1:
		CheckDistance(world, entity, phase);
		Attack(world, entity, phase);
		break;
	case 2:
		CheckDistance(world, entity, phase);
		break;
	}

	return 0;
}

int LMAttack::Attack(World* world, Entity* entity, int phase)
{
	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (evnt->needToMove)
	{
		if (entity->GetStartedAttack())
		{
			evnt->needToMove = false;
		}
		else
		{
			//entity->SetStartedAttack(false);
			return 0;
		}
	}
	else if (!entity->GetStartedAttack())
	{
		//evnt->initialized = true;
		entity->SetAnimationProgress(0.0f);
		entity->ResetAnimationSecond();
		entity->SetStartedAttack(true);
		entity->SetAttackTimer(SDL_GetTicks());

		evnt->path.Clear();
		evnt->initialized = false;
		evnt->requestedPath = false;
	}

	//Attack entity

	//First adjust the angle to face the target
	Entity* targetEntity = evnt->entity;
	entity->SetRotation(-glm::degrees(atan2(targetEntity->PosZ()-entity->PosZ(), targetEntity->PosX()-entity->PosX()))+90.0f);

	//Check if any projectiles need to be created during the attack
	unsigned int projectileSize = entity->GetEntityBase()->GetEntityXML()->attackVector[0].projectileVector.size();
	for (int i = 0; i < (int)projectileSize; i++)
	{
		EntityXMLTag::Projectile projXML = entity->GetEntityBase()->GetEntityXML()->attackVector[0].projectileVector[i];
		//if (entity->GetAnimationProgress() > projectile.frame && !entity->GetProjectileCreated(i))
		float animationSeconds = ((float)(SDL_GetTicks() - entity->GetAttackTimer())) / 1000.0f;
		float attackSpeed = entity->GetEntityBase()->GetEntityXML()->attackVector[0].attackSpeed;
		if (animationSeconds / attackSpeed > projXML.frame && !entity->GetProjectileCreated(i))
		{
			DebugOutput("CREATE PROJECTILE %s FOR ENTITY: %s\n", projXML.name.c_str(), entity->GetEntityBase()->EntityName().c_str());
			entity->SetProjectileCreated(i, true);

			Projectile* projectile = new Projectile();
			glm::vec3 origin = entity->GetTranslationVector();
			origin += entity->GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint();
			float distance = sqrt(pow(evnt->entity->PosX() - entity->PosX(), 2.0f) + pow(evnt->entity->PosZ() - entity->PosZ(), 2.0f));
			projectile->CreateProjectile(origin, 5.0f * (distance/8.0f), entity, targetEntity, projXML.velocity, entity->GetEntityBase()->GetEntityXML()->attackVector[0].damage);
			world->GetProjectileSystem()->AddProjectile(projectile);
		}

		if (animationSeconds >= attackSpeed)
		{
			entity->SetAttackTimer(SDL_GetTicks());
			entity->ClearAllProjectileCreated();
			entity->SetStartedAttack(false);
		}
	}

	return 0;
}

int LMAttack::CheckDistance(World* world, Entity* entity, int phase)
{
	if (entity->GetStartedAttack())
	{
		return 0;
	}

	EntityEvent* evnt = entity->GetEventQueue()->GetFrontEvent();

	if (phase == 0)
	{
		//Calculate the distance
		float distance = sqrt(pow(evnt->entity->PosX() - entity->PosX(), 2.0f) + pow(evnt->entity->PosZ() - entity->PosZ(), 2.0f));

		//Set whether movement is needed for this event
		if (distance > 8.0f/*ADDVAR: Min distance to attack*/)
			evnt->needToMove = true;
		else
			evnt->needToMove = false;

		//If we need to move and there is no path, or the target entity has moved off target, then create a new path.
		if (evnt->needToMove && (evnt->path.Size() <= 0 ||
			sqrt(pow(evnt->entity->PosX() - evnt->path.Back().x, 2.0f) + pow(evnt->entity->PosZ() - evnt->path.Back().y, 2.0f)) > 8.0f /*ADDVAR: Min distance to build times 2*/))
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

	return 0;
}
