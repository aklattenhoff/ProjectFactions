#ifndef _SHADOW_MAP_H
#define _SHADOW_MAP_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include "Game/GUI/GUIIcon.h"
//#include "Graphics/Camera.h"
#include "Game/World.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	int Init(World* world);
	//int CalcFrustumPlaneIntersect(Camera* camera);
	int RenderTest(World* world);
	int Clear();
private:
	//Framebuffer
	GLuint FBO;
	GLuint RBO;
	//GLuint fbTexture;
	GLuint fbDepth;

	GUIIcon* guiScreen;

	GUIIcon* guiIcon;
	GLuint circleTexture;

	glm::vec2 _corners[4];
	float _angle;

	float _translateX;
	float _translateY;
};
#endif
