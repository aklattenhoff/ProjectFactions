#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Factions/Factions.h"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#pragma comment(lib, "freetype27.lib")
#pragma comment(lib, "lib3ds-2_0.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "soloud_static.lib")

int main(int argc, char* argv[])
{
	//Create main Factions object
	Factions *factions = new Factions();

	//Load and parse the factions.xml file
	factions->ParseMainXML();

	//Initialize the graphics for the program
	factions->CreateWindowAndGLContext();

	//Load all glext functions for the program
	//DEP: This no longer happens in the factions object - moved to Renderer
	//factions->InitGLExtensions();

	if (factions->Init() == 0)
		factions->StartProgram();

	//Delete Factions object
	delete factions;

	return 0;
}