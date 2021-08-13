#ifndef _GAME_RESPONSE_H
#define _GAME_RESPONSE_H

#include <string>

class GameResponse
{
public:
	GameResponse();
	~GameResponse();

	int Init(std::string name, unsigned int start, unsigned int life);

	std::string GetGameName() {return _gameName;}
	unsigned int GetEndTime() {return _startTime+_lifeTime;}
	int SetNewStartTime(unsigned int start);
private:
	std::string _gameName;
	unsigned int _startTime;
	unsigned int _lifeTime;
};

#endif
