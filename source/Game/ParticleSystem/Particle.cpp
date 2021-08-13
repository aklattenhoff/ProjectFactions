#include "Particle.h"

Particle::Particle()
{
}

Particle::~Particle()
{
}

int Particle::Init(GLuint texture, glm::vec3 pos, glm::vec3 vel, glm::vec3 acc, float rot, float scale, float scaleSpeed, int life)
{
	_texture = texture;
	_position = pos;
	_velocity = vel;
	_acceleration = acc;
	_rotation = rot;
	_scale = scale;
	_scaleSpeed = scaleSpeed;
	_life = life;
	_alpha = (float)life;

	return 0;
}

int Particle::Step()
{
	_velocity += _acceleration;
	_position += _velocity;
	_scale += _scaleSpeed;
	_life --;

	return 0;
}

GLuint Particle::GetTexture()
{
	return _texture;
}

glm::vec3 Particle::GetPosition()
{
	return _position;
}

float Particle::GetRotation()
{
	return _rotation;
}

float Particle::GetScale()
{
	return _scale;
}

bool Particle::IsDead()
{
	return (_life <= 0);
}

float Particle::GetAlpha()
{
	return (float)_life / _alpha;
}
