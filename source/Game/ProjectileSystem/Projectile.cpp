#include "Projectile.h"

#include "Renderer/Renderer.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityBase.h"

Projectile::Projectile()
{
	_projectileEntity = NULL;
	_currentAdditionalHeight = 0.0f;
}

Projectile::~Projectile()
{
	delete _projectileEntity;
}

int Projectile::CreateProjectile(glm::vec3 origin, float additionalHeight, Entity* originEntity, Entity* targetEntity, float velocity, float damage)
{
	_originPos = origin;
	_additionalHeight = additionalHeight;
	_originEntity = originEntity;
	_targetEntity = targetEntity;
	_velocity = velocity;
	_damage = damage;

	_projectilePos = _originPos;
	glm::vec3 taPos = _targetEntity->GetTranslationVector() + _targetEntity->GetEntityBase()->BoundingBox3D()->CalcAverageMidpoint();
	_initialDistanceToTarget = glm::length(taPos - _originPos);

	_projectileEntity = new Phage::PhageEntity();
	_projectileEntity->Init();
	//_projectileEntity->SetPhageModel(projectileModel);
	_projectileEntity->SetBaseRotate(-90.0f, 1.0f, 0.0f, 0.0f);
	_projectileEntity->AddScale(0.2f, 0.2f, 0.2f);

	return 0;
}

int Projectile::SetProjectileBase(ProjectileBase* projectileBase)
{
	_projectileBase = projectileBase;

	return 0;
}

int Projectile::SetProjectileModel(Phage::PhageModel* model)
{
	_projectileEntity->SetPhageModel(model);

	return 0;
}

int Projectile::DrawProjectile()
{
	_projectileEntity->SetTranslate(_projectilePos.x, _projectilePos.y + _currentAdditionalHeight, _projectilePos.z);
	RendererSingleton->AddPhageEntityNonRS(_projectileEntity);

	return 0;
}
