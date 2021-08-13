#include "EntityAnimationHandler.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"
#include "Phage/Backend/Model.h"
#include "Phage/Backend/Animation.h"

#include "Renderer/Renderer.h"

EntityAnimationHandler::EntityAnimationHandler()
{
}

EntityAnimationHandler::~EntityAnimationHandler()
{
}

int EntityAnimationHandler::Init()
{
	return 0;
}

int EntityAnimationHandler::ProcessAnimation(Entity* entity)
{
	if (!entity->GetModel()->HasAnimation())
		return -1;

	Animation* currentAnimation = entity->GetEntityBase()->GetPhageModel()->GetAnimation(entity->GetAnimationIndex());

	float currentSeconds = entity->GetAnimationSeconds();
	float durationSeconds = entity->GetModel()->GetAnimation(entity->GetAnimationIndex())->GetDurationInSeconds();
	float animationSpeed = entity->GetModel()->GetAnimation(entity->GetAnimationIndex())->GetAnimationSpeed();
	currentSeconds *= animationSpeed;
	
	bool hasChangedStatus = entity->HasEntityStatusChanged();
	//if (hasChangedStatus)
		//DebugOutput("EntityStatus Has Changed\n");
	entity->SetCachedEntityStatus(entity->GetEntityStatus());

	int random = rand() % 100;

	bool checkForProjectileCreation = false;
	EntityXMLTag::Projectile proj;
	if (entity->GetEntityStatus() == Entity::STATUS_ATTACKING)
	{
		//proj = entity->GetEntityBase()->GetEntityXML()->attackVector[0].projectileVector[0];
		//checkForProjectileCreation = (entity->GetAnimationProgress() < proj.frame);
	}
		
	//Update animation progress
	int animationCycle = (int)floor(currentSeconds / durationSeconds);
	float progress = currentSeconds / durationSeconds;
	if (progress > 1.0) progress -= floor(progress);
	if (hasChangedStatus) progress = 0.0f;

	float prevProgress = entity->GetAnimationProgress();
	if (hasChangedStatus) prevProgress = 0.0f;
	float rawProgress = progress;
	if (rawProgress < prevProgress) rawProgress += 1.0f;

	entity->SetAnimationProgress(progress);

	//DebugOutput("SFX Size: %i\n", currentAnimation->GetSoundEffectSize());
	for (int i = 0; i < currentAnimation->GetSoundEffectSize(); i++)
	{
		float sfxTime = currentAnimation->GetSoundEffectTime(i);
		if (!hasChangedStatus && sfxTime > prevProgress && sfxTime < rawProgress)
		{
			DebugOutput("PLAY SOUND: %f < %f < %f\n", prevProgress, sfxTime, rawProgress);
			DebugOutput("Sound to play: %s\n", currentAnimation->GetSoundEffectName(i).c_str());
			Phage::PhageSound* soundToPlay = entity->GetEntityBase()->GetPhageModel()->GetPhageSound(currentAnimation->GetSoundEffectName(i));
			RendererSingleton->PlaySound(soundToPlay);
		}
	}

	//If a new animation cycle started this frame, reset the animation cycle dependent values
	if (animationCycle > entity->GetAnimationCycle())
	{
		//entity->ClearAllProjectileCreated();
		entity->SetAnimationCycle(animationCycle);
	}

	//This is the case when the entity is dying or dead, we just set the animation and return
	if (!entity->IsAlive())
	{
		if (entity->IsDying())
		{
			if (!entity->StartedDeathAnimation())
			{
				DebugOutput("Dying\n");
				entity->SetAnimationIndex(Model::ANIMATION_DEATH);
				entity->ResetAnimationSecond();
				entity->SetStartedDeathAnimation(true);
			}
			else
			{
				if (currentSeconds > durationSeconds)
				{
					DebugOutput("Dead: %f\n", durationSeconds);
					entity->SetAnimationIndex(Model::ANIMATION_DEAD);
					entity->ResetAnimationSecond();
					entity->SetIsDying(false);
				}
			}
		}

		return 0;
	}

	switch (entity->GetEntityStatus())
	{
	case Entity::STATUS_IDLING:
		if (hasChangedStatus)
		{
			entity->SetAnimationIndex(Model::ANIMATION_IDLE_1);
			entity->ResetAnimationSecond();
		}
		else if (currentSeconds > durationSeconds)
		{
			if (random < 30)
			{
				if (entity->GetModel()->GetAnimation(Model::ANIMATION_BORED_1))
					entity->SetAnimationIndex(Model::ANIMATION_BORED_1);
			}
			else if (random >= 30 && random < 60)
			{
				if (entity->GetModel()->GetAnimation(Model::ANIMATION_IDLE_2))
					entity->SetAnimationIndex(Model::ANIMATION_IDLE_2);
			}
			else if (random >= 60)
			{
				if (entity->GetModel()->GetAnimation(Model::ANIMATION_IDLE_1))
					entity->SetAnimationIndex(Model::ANIMATION_IDLE_1);
			}
			entity->ResetAnimationSecond();
		}
		break;
	case Entity::STATUS_WALKING:
		if (hasChangedStatus)
		{
			entity->SetAnimationIndex(Model::ANIMATION_WALKING);
			entity->ResetAnimationSecond();
		}
		break;
	case Entity::STATUS_GATHERING:
		if (hasChangedStatus)
		{
			if (entity->GetGatherOverwriteType() != -1 && entity->GetEntityBase()->GetPhageModel()->GetAnimation(entity->GetGatherOverwriteType()))
			{
				entity->SetAnimationIndex(entity->GetGatherOverwriteType());
				DebugOutput("FOUND OVER ANIM: %i\n", entity->GetGatherOverwriteType());
			}
			else if (entity->GetGatherType() != -1 && entity->GetEntityBase()->GetPhageModel()->GetAnimation(entity->GetGatherType()))
			{
				entity->SetAnimationIndex(entity->GetGatherType());
				DebugOutput("FOUND ANIM: %i\n", entity->GetGatherType());
			}
			else
			{
				entity->SetAnimationIndex(Model::ANIMATION_GATHERING_FOOD);
				DebugOutput("NO ANIM: %i, %i\n", entity->GetGatherType(), entity->GetGatherOverwriteType());
			}
			entity->ResetAnimationSecond();
		}
		break;
	case Entity::STATUS_ATTACKING:
		if (hasChangedStatus)
		{
			entity->SetAnimationIndex(Model::ANIMATION_ATTACK_1);
			entity->ResetAnimationSecond();
		}
		{
			//Check if we need to create a projectile in this frame of the animation

			//if (checkForProjectileCreation && progress > proj.frame)
			{
				//Create the projectile
				//entity->SetNeedsToCreateProjectile(true);
			}
		}
		break;
	case Entity::STATUS_BUILDING:
		if (hasChangedStatus)
		{
			entity->SetAnimationIndex(Model::ANIMATION_BUILD);
			entity->ResetAnimationSecond();
		}
		break;
	default:
		entity->SetAnimationIndex(Model::ANIMATION_IDLE_1);
		break;
	}

	return 0;
}
