#include "Menu.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

int Menu::Init()
{
	return 0;
}

unsigned int Menu::GetTime()
{
	return _time;
}

int Menu::SetTime(unsigned int t)
{
	_time = t;

	return 0;
}

Input* Menu::GetInput()
{
	return _input;
}

int Menu::SetInput(Input* i)
{
	_input = i;

	return 0;
}

/*Camera* Menu::GetCamera()
{
	return _camera;
}

int Menu::SetCamera(Camera* c)
{
	_camera = c;

	return 0;
}*/

Game* Menu::GetGame()
{
	return _game;
}

int Menu::SetGame(Game* g)
{
	_game = g;

	return 0;
}

Network* Menu::GetNetwork()
{
	return _network;
}

int Menu::SetNetwork(Network* n)
{
	_network = n;

	return 0;
}
