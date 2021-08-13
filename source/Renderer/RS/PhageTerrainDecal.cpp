#include "PhageTerrainDecal.h"

#include "Game/Player/Player.h"
#include "Game/Entity/EntityBase.h"

RS::PhageTerrainDecal::PhageTerrainDecal()
{
}

RS::PhageTerrainDecal::~PhageTerrainDecal()
{

}

int RS::PhageTerrainDecal::SetTerrainDecal(Phage::PhageTerrainDecal* phTerrainDecal, glm::vec3 pos, float rot)
{
	_phageTerrainDecal = phTerrainDecal;
	_position = pos;
	_rotation = rot;

	return 0;
}

int RS::PhageTerrainDecal::SetPlayer(Player* player)
{
	_player = player;

	return 0;
}

int RS::PhageTerrainDecal::SetEntityBase(EntityBase* entBase)
{
	_entityBase = entBase;

	return 0;
}

Phage::PhageTerrainDecal* RS::PhageTerrainDecal::GetTerrainDecal()
{
	return _phageTerrainDecal;
}

glm::vec3 RS::PhageTerrainDecal::GetPosition()
{
	return _position;
}

float RS::PhageTerrainDecal::GetRotation()
{
	return _rotation;
}

Player* RS::PhageTerrainDecal::GetPlayer()
{
	return _player;
}

EntityBase* RS::PhageTerrainDecal::GetEntityBase()
{
	return _entityBase;
}
