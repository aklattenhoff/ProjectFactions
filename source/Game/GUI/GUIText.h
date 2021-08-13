#ifndef _GUI_TEXT_H
#define _GUI_TEXT_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftstroke.h>

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/GUI/OrthoDepth.h"

//#include "Graphics/Camera.h"

class GUIText
{
public:
	GUIText();
	~GUIText();

	int Init(char* filename, int size, int dpi = 0);
	//int Draw(int x, int y, std::string str, Camera* camera, std::vector<unsigned int>* resourceTexVector = NULL);
	float GetStringHeight();
	glm::vec2 CalcStringSize(std::string str);
	int Destroy();
private:
	int CreateGlyph(FT_Face face, FT_Stroker stroker, char c, GLuint* tex);
private:
	float* textVertices;
	GLuint* textIndices;
	float* textTexCoords;
	GLuint textVAO;
	GLuint textVBO;
	GLuint textIBO;
	GLuint textTBO;

	GLuint* glyphs;
	FT_BBox* boundingBoxes;
	int* glyphWidths;
	int* glyphHeights;
	int fontHeight;
};
#endif
