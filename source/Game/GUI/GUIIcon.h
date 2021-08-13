#ifndef _GUI_ICON_H
#define _GUI_ICON_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/GUI/OrthoDepth.h"

//#include "Graphics/Camera.h"

class GUIIcon
{
public:
	GUIIcon();
	~GUIIcon();

	int Init();
	int Init(std::string fileName);
	int Generate(glm::vec2 size);
	int Draw();
	int Draw(GLuint tex);
	//int DrawAtLocation(Camera* camera, float x, float y, GLuint tex, float depth = 0.1f);
	int DrawAtLocation();
	glm::vec2 GetSize();
private:
	float* vertices;
	GLuint* indices;
	float* texCoords;
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLuint TBO;
	GLuint texture;
};
#endif
