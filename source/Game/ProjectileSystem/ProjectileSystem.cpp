#include "ProjectileSystem.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/ProjectileSystem/Projectile.h"
#include "Renderer/Renderer.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"

ProjectileSystem::ProjectileSystem()
{
}

ProjectileSystem::~ProjectileSystem()
{
}

int ProjectileSystem::Init()
{
	_projectileModel = new Phage::PhageModel();
	_projectileModel->Init();
	if (_projectileModel->LoadCollada("models/projectile.dae") == -1)
		return 0;
	if (_projectileModel->LoadMainTexture("models/projectile.png") == -1)
		return 0;
	if (_projectileModel->LoadBumpTexture("models/projectile_bump.png") == -1)
		return 0;
	if (_projectileModel->LoadSBCTexture("models/projectile_sbc.png") == -1)
		return 0;
	_projectileModel->SetOrientation(Phage::PhageModel::ORIENTATION_Z);
	_projectileModel->SetScale(0.2f);

	return 0;
}

int ProjectileSystem::AddProjectile(Projectile* projectile)
{
	_projectileVector.push_back(projectile);

	projectile->SetProjectileModel(_projectileModel);

	return 0;
}

int ProjectileSystem::ProcessProjectiles(World* world)
{
	for (int i = 0; i < (int)_projectileVector.size(); i++)
	{
		Projectile* projectile = _projectileVector[i];
		glm::vec3 prPos = projectile->_projectilePos;
		glm::vec3 taPos = projectile->_targetEntity->GetTranslationVector() + projectile->_targetEntity->GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint();

		//Calculate vector from projectile to target
		glm::vec3 toTargetVector = taPos - prPos;
		float distanceToMoveThisFrame = projectile->_velocity * world->GetDeltaSeconds();
		//If we will reach the target this frame, then calc projectile collision
		if (glm::length(toTargetVector) < distanceToMoveThisFrame)
		{
			//Deal the damage here
			float damageToDeal = projectile->_damage;
			projectile->_targetEntity->AddPreMitigatedDamage(damageToDeal, 1);

			//Delete the projectile
			delete projectile;
			_projectileVector.erase(_projectileVector.begin() + i);
			i--;
		}
		else
		{
			projectile->_projectilePos += glm::normalize(toTargetVector) * distanceToMoveThisFrame;

			//Calculate the estimated progress to target
			float progressToTarget = 1.0f - (glm::length(toTargetVector) / projectile->_initialDistanceToTarget);
			float sinHeightRatio = glm::sin(progressToTarget * 3.1415926f);
			float finalHeight = sinHeightRatio * projectile->_additionalHeight;
			projectile->_currentAdditionalHeight = finalHeight;
		}
	}

	return 0;
}

int ProjectileSystem::DrawProjectiles(World* world)
{
	for (int i = 0; i < (int)_projectileVector.size(); i++)
	{
		Projectile* projectile = _projectileVector[i];

		//RendererSingleton->AddLineDrawInfo(projectile->_originPos, projectile->_projectilePos);

		projectile->DrawProjectile();
	}

	return 0;
}
