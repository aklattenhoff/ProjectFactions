#ifndef _MENU_H
#define _MENU_H

#include "Game/Input/Input.h"
//#include "Graphics/Camera.h"
#include "Game/Game.h"
#include "Network/Network.h"

class Menu
{
public:
	Menu();
	~Menu();

	int Init();

	//Returns the most recently set time.
	//This is not the current time but a close approximate
	unsigned int GetTime();
	int SetTime(unsigned int t);

	Input* GetInput();
	int SetInput(Input* i);

	//Camera* GetCamera();
	//int SetCamera(Camera* c);

	Game* GetGame();
	int SetGame(Game* g);

	Network* GetNetwork();
	int SetNetwork(Network* n);
private:
	unsigned int _time;
	Input* _input;
	//Camera* _camera;
	Game* _game;
	Network* _network;
};

#endif
