#include "Player.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/EntityList.h"

Player::Player()
{
	entityList = NULL;
	_resources.food = 0.0f;
	_resources.wood = 0.0f;
	_resources.stone = 0.0f;
	_resources.alloy = 0.0f;
	_resources.gold = 0.0f;
	_resources.mana = 0.0f;
}

Player::~Player()
{
	delete entityList;
}

int Player::Init()
{
	entityList = new EntityList();

	return 0;
}

EntityList* Player::GetEntityList()
{
	return entityList;
}

int Player::SetColor(float r, float g, float b)
{
	_color.red = r;
	_color.green = g;
	_color.blue = b;

	return 0;
}

int Player::SetColor(Player::Color color)
{
	_color = color;

	return 0;
}

Player::Color Player::GetColor()
{
	return _color;
}

int Player::SetTeam(int t)
{
	_team = t;

	return 0;
}

int Player::GetTeam()
{
	return _team;
}

Player::Resources Player::GetResources()
{
	return _resources;
}

int Player::ModifyResource(int resource, float value)
{
	switch (resource)
	{
	case 0: _resources.food += value; break;
	case 1: _resources.wood += value; break;
	case 2: _resources.stone += value; break;
	case 3: _resources.alloy += value; break;
	case 4: _resources.gold += value; break;
	case 5: _resources.mana += value; break;
	default: break;
	}

	return 0;
}

int Player::GetResourceIndex(std::string str)
{
	int returnInt = -1;

	if (str.compare("food") == 0)
		returnInt = 0;
	else if (str.compare("wood") == 0)
		returnInt = 1;
	else if (str.compare("stone") == 0)
		returnInt = 2;
	else if (str.compare("alloy") == 0)
		returnInt = 3;

	return returnInt;
}

float Player::GetResourceValue(std::string str)
{
	float returnFloat = -1;

	if (str.compare("food") == 0)
		returnFloat = _resources.food;
	else if (str.compare("wood") == 0)
		returnFloat = _resources.wood;
	else if (str.compare("stone") == 0)
		returnFloat = _resources.stone;
	else if (str.compare("alloy") == 0)
		returnFloat = _resources.alloy;

	return returnFloat;
}
