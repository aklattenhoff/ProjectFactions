#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

//#include "Graphics/Utility/Quad.h"
#include "Game/ParticleSystem/Particle.h"

#include "Game/World.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	int Init();
	int Draw(World* world, float camRotation);
private:
	//Quad* _quad;
	std::vector<Particle*> _particleVector;

	int _testTimer;
	GLuint _cloudTexture;
};
#endif
