#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Input/Input.h"
//#include "Graphics/Camera.h"
#include "Game/Game.h"
#include "Game/GUI/GUIText.h"
#include "Renderer/RS/GUITexture.h"

#include "MainMenu/Menu.h"
#include "MainMenu/MMLANLobby.h"
#include "MainMenu/MMLANHost.h"

#include "MainMenu/UIElements/MMPage.h"

#include "Network/Network.h"
#include "Texturer/Texturer.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	int InitCamera(glm::vec2 screenResolution);
	int Init();
	int SetWindow(SDL_Window* w);
	int SetNetwork(Network* n);
	int SetGame(Game* g);
	int Run();
private:
	int Frame();
private:
	static const int MM_LAN_LOBBY = 0;
	static const int MM_LAN_HOST = 1;
private:
	SDL_Window* _window;
	Network* _network;
	Game* _game;
	SDL_Event sdlEvent;

	Input* _input;
	//Camera* _camera;
	GUIText* _guiText;
	Texturer::Texture _textureCursor;
	RS::GUITexture _rsGUITextureCursor;

	Menu* _menu;
	MMLANLobby* _mmLANLobby;
	MMLANHost* _mmLANHost;
	int _page;

	MMPage* _mmPage;

	float FPS;
	unsigned int ticks;
};
#endif
