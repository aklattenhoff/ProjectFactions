#ifndef _SQUADRON_MANAGER_H
#define _SQUADRON_MANAGER_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class World;
class Squadron;

class SquadronManager
{
public:
	SquadronManager();
	~SquadronManager();

	int Init();
	Squadron* CreateSquadron();
	int Draw(World* world);
private:
	std::vector<Squadron*> _squadronVector;

	GLuint _debugTexture;
	GLuint _debugGroupTexture;
};

#endif
