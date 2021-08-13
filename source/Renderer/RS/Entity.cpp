#include "Entity.h"

#include "Game/Player/Player.h"
#include "Game/Entity/EntityBase.h"

RS::Entity::Entity()
{
	_animationIndex = 0;
	_animationTime = 0.0f;
}

RS::Entity::~Entity()
{

}

int RS::Entity::SetModel(Model* model, glm::vec3 pos, float rot)
{
	_model = model;
	_position = pos;
	_rotation = rot;

	return 0;
}

int RS::Entity::SetAnimation(int animIndex, float animTime)
{
	_animationIndex = animIndex;
	_animationTime = animTime;

	return 0;
}

int RS::Entity::SetPlayer(Player* player)
{
	_player = player;

	return 0;
}

int RS::Entity::SetEntityBase(EntityBase* entBase)
{
	_entityBase = entBase;

	return 0;
}

Model* RS::Entity::GetModel()
{
	return _model;
}

glm::vec3 RS::Entity::GetPosition()
{
	return _position;
}

float RS::Entity::GetRotation()
{
	return _rotation;
}

int RS::Entity::GetAnimationIndex()
{
	return _animationIndex;
}

float RS::Entity::GetAnimationTime()
{
	return _animationTime;
}

Player* RS::Entity::GetPlayer()
{
	return _player;
}

EntityBase* RS::Entity::GetEntityBase()
{
	return _entityBase;
}
