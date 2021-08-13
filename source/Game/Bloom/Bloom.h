#ifndef _BLOOM_H
#define _BLOOM_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "Game/GUI/GUIIcon.h"
//#include "Graphics/Camera.h"
#include "Game/World.h"

class Bloom
{
public:
	Bloom();
	~Bloom();

	int Init();
	int RenderTest(World* world);
	int Clear();
private:
	//Framebuffer
	GLuint FBO;
	GLuint RBO;
	GLuint fbTexture;
	GLuint fbDepth;
	GLuint sc1FBO;
	GLuint sc1Texture;
	GLuint sc2FBO;
	GLuint sc2Texture;

	GUIIcon* guiScreen;

	GUIIcon* guiIcon;
	GLuint circleTexture;
};
#endif
