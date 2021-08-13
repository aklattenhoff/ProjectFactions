#ifndef _GAME_CREATOR_H
#define _GAME_CREATOR_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GameCreator
{
public:
	GameCreator();
	~GameCreator();

	int Init();
	int SetPlayerData(int numOfPlayers);
	int CreateGame(std::string mapFile);
private:
	//This is temporary, it should be changed with actual Player object data
	int _numberOfPlayers;

	//
};
#endif
