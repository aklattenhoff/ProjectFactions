#ifndef _PLAYER_LIST_H
#define _PLAYER_LIST_H

#include <vector>

class Player;

class PlayerList
{
public:
	PlayerList();
	~PlayerList();

	int Init();

	int CreateAndAdd();

	Player* GetAt(int i);

	int SetGameOwner(int i);
	int GetGameOwner();

private:
	std::vector<Player*> _playerVector;
	int _gameOwner;
};

#endif
