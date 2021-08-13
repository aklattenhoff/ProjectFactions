#ifndef _PHAGE_UI_TEXT_H
#define _PHAGE_UI_TEXT_H

#include <stdlib.h>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftstroke.h>

#include "Phage_Matrix.h"

namespace Phage
{
	class PhageUIText
	{
	public:
		PhageUIText();
		~PhageUIText();

		int Init();
		int CreateText(std::string filename, int size, int dpi);
		glm::mat4 GetMatrix();

		int AddUITextIcon(char c, unsigned int texture);

		int Draw(int x, int y, std::string str);
	private:
		int CreateGlyph(FT_Face face, FT_Stroker stroker, char c, GLuint* tex);
	private:
		Phage::Matrix _matrix;

		float* _textVertices;
		GLuint* _textIndices;
		float* _textTexCoords;
		GLuint _textVAO;
		GLuint _textVBO;
		GLuint _textIBO;
		GLuint _textTBO;

		GLuint* _glyphs;
		FT_BBox* _boundingBoxes;
		int* _glyphWidths;
		int* _glyphHeights;
		int _fontHeight;

		struct TextIcon {char c; unsigned int tex;};
		std::vector<TextIcon> _textIconVector;
	};
}

#endif
