#include "PhageEntity.h"

#include "Game/Player/Player.h"
#include "Game/Entity/EntityBase.h"
#include "Renderer/RS/Prop.h"

RS::PhageEntity::PhageEntity()
{
	_animationIndex = 0;
	_animationTime = 0.0f;
}

RS::PhageEntity::~PhageEntity()
{

}

int RS::PhageEntity::SetModel(Phage::PhageEntity* phEnt, glm::vec3 pos, float rot)
{
	_phageEntity = phEnt;
	_position = pos;
	_rotation = rot;

	return 0;
}

int RS::PhageEntity::SetAnimation(int animIndex, float animTime)
{
	_animationIndex = animIndex;
	_animationTime = animTime;

	return 0;
}

int RS::PhageEntity::SetPlayer(Player* player)
{
	_player = player;

	return 0;
}

int RS::PhageEntity::SetEntityBase(EntityBase* entBase)
{
	_entityBase = entBase;

	return 0;
}

Phage::PhageEntity* RS::PhageEntity::GetModel()
{
	return _phageEntity;
}

glm::vec3 RS::PhageEntity::GetPosition()
{
	return _position;
}

float RS::PhageEntity::GetRotation()
{
	return _rotation;
}

int RS::PhageEntity::GetAnimationIndex()
{
	return _animationIndex;
}

float RS::PhageEntity::GetAnimationTime()
{
	return _animationTime;
}

Player* RS::PhageEntity::GetPlayer()
{
	return _player;
}

EntityBase* RS::PhageEntity::GetEntityBase()
{
	return _entityBase;
}

int RS::PhageEntity::AddProp(RS::Prop prop)
{
	_propVector.push_back(prop);

	return 0;
}

int RS::PhageEntity::PropSize()
{
	return _propVector.size();
}

RS::Prop RS::PhageEntity::PropAt(int index)
{
	return _propVector[index];
}
