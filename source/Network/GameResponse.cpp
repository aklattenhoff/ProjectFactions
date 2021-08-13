#include "GameResponse.h"

GameResponse::GameResponse()
{
}

GameResponse::~GameResponse()
{
}

int GameResponse::Init(std::string name, unsigned int start, unsigned int life)
{
	_gameName = name;
	_startTime = start;
	_lifeTime = life;

	return 0;
}

int GameResponse::SetNewStartTime(unsigned int start)
{
	_startTime = start;

	return 0;
}
