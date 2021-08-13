#include "PlayerList.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Player/Player.h"

PlayerList::PlayerList()
{
}

PlayerList::~PlayerList()
{
}

int PlayerList::Init()
{
	return 0;
}

int PlayerList::CreateAndAdd()
{
	_playerVector.push_back(new Player());
	_playerVector.back()->Init();

	return 0;
}

Player* PlayerList::GetAt(int i)
{
	return _playerVector.at(i);
}

int PlayerList::SetGameOwner(int i)
{
	_gameOwner = i;

	return 0;
}

int PlayerList::GetGameOwner()
{
	return _gameOwner;
}
