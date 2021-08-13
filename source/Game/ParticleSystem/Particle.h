#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Particle
{
public:
	Particle();
	~Particle();

	int Init(GLuint texture, glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float rot, float scale, float scaleSpeed, int life);

	int Step();

	GLuint GetTexture();
	glm::vec3 GetPosition();
	float GetRotation();
	float GetScale();
	bool IsDead();
	float GetAlpha();
private:
	GLuint _texture;
	glm::vec3 _position;
	glm::vec3 _velocity;
	glm::vec3 _acceleration;
	float _rotation;
	float _scale;
	float _scaleSpeed;
	int _life;
	float _alpha;
};
#endif
