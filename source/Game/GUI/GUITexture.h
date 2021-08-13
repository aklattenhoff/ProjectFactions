#ifndef _GUI_TEXTURE_H
#define _GUI_TEXTURE_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GUITexture
{
public:
	GUITexture();
	~GUITexture();

	int Draw(GLuint tex);

	int SetPoints(glm::vec4 points, float depth = 0.1);
private:
	float* guiTextureVertices;
	GLuint* guiTextureIndices;
	float* guiTextureTexCoords;
	GLuint guiTextureVAO;
	GLuint guiTextureVBO;
	GLuint guiTextureIBO;
	GLuint guiTextureTBO;
};
#endif
