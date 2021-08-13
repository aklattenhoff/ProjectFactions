#ifndef _PROJECTILE_SYSTEM_H
#define _PROJECTILE_SYSTEM_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "Game/World.h"
#include "Phage/Phage.h"

class Projectile;

class ProjectileSystem
{
public:
	ProjectileSystem();
	~ProjectileSystem();

	int Init();
	int AddProjectile(Projectile* projectile);
	int ProcessProjectiles(World* world);
	int DrawProjectiles(World* world);
private:
	std::vector<Projectile*> _projectileVector;

	Phage::PhageModel* _projectileModel;
};
#endif
