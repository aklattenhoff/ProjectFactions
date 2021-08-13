#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Phage/Phage.h"

class Entity;

class ProjectileBase;

class Projectile
{
public:
	Projectile();
	~Projectile();

	int CreateProjectile(glm::vec3 origin, float additionalHeight, Entity* originEntity, Entity* targetEntity, float velocity, float damage);
	int SetProjectileBase(ProjectileBase* projectileBase);

	int SetProjectileModel(Phage::PhageModel* model);
	int DrawProjectile();
private:
	friend class ProjectileSystem;

	ProjectileBase* _projectileBase;

	glm::vec3 _originPos;
	float _additionalHeight;
	Entity* _originEntity;
	Entity* _targetEntity;
	float _velocity;
	float _damage;

	glm::vec3 _projectilePos;
	float _initialDistanceToTarget;
	float _currentAdditionalHeight;

	Phage::PhageEntity* _projectileEntity;
};
#endif
