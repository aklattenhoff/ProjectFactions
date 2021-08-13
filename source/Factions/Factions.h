#ifndef _FACTIONS_H
#define _FACTIONS_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "MainMenu/MainMenu.h"

#include "Game/Game.h"
//#include "Game/Terrain/Terrain.h"

#include "Network/Network.h"

class Parser;

extern const char* FACTIONS_XML_FILENAME;

//Main program container
class Factions
{
public:
	Factions();
	~Factions();

	//Loads the factions.xml file and parses it into the FactionsXMLParser object
	int ParseMainXML();

	//Initializes the graphics and window for the program
	int CreateWindowAndGLContext();

	//Loads all glext functions needed for the program
	int InitGLExtensions();

	int Init();

	int StartProgram();

	//A single frame of the program.
	//This is mainly used for the menu system of the program
	//When a game is created, it will have it's own program loop
	int Frame();
private:
	MainMenu* _mainMenu;

	//Window object
	SDL_Window* window;

	//OpenGL context object
	SDL_GLContext glContext;

	//SDL Event object used for event polling
	SDL_Event sdlEvent;

	Game* _game;

	Network* _network;
	Parser* _parser;
};
#endif
