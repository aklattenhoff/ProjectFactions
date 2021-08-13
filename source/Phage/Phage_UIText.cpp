#include "Phage_UIText.h"

#include "loadGLE.h"
#include "Debug/Debug.h"


Phage::PhageUIText::PhageUIText()
{
	_glyphs = NULL;
	_boundingBoxes = NULL;
	_glyphWidths = NULL;
	_glyphHeights = NULL;
}

Phage::PhageUIText::~PhageUIText()
{
	delete[] _textVertices;
	delete[] _textIndices;
	delete[] _textTexCoords;
	delete[] _glyphs;
	delete[] _boundingBoxes;
	delete[] _glyphWidths;
	delete[] _glyphHeights;
}

int Phage::PhageUIText::Init()
{
	_matrix.Identity();

	_textVertices = new float[12];

	_textVertices[0] = 0.0f;
	_textVertices[1] = 0.0f;
	_textVertices[2] = 0.99f;

	_textVertices[3] = 1.0f;
	_textVertices[4] = 0.0f;
	_textVertices[5] = 0.99f;

	_textVertices[6] = 1.0f;
	_textVertices[7] = 1.0f;
	_textVertices[8] = 0.99f;

	_textVertices[9] = 0.0f;
	_textVertices[10] = 1.0f;
	_textVertices[11] = 0.99f;

	_textIndices = new GLuint[6];

	_textIndices[0] = 0;
	_textIndices[1] = 2;
	_textIndices[2] = 1;
	_textIndices[3] = 0;
	_textIndices[4] = 3;
	_textIndices[5] = 2;

	_textTexCoords = new float[8];

	_textTexCoords[0] = 0.0f;
	_textTexCoords[1] = 0.0f;

	_textTexCoords[2] = 1.0f;
	_textTexCoords[3] = 0.0f;

	_textTexCoords[4] = 1.0f;
	_textTexCoords[5] = 1.0f;

	_textTexCoords[6] = 0.0f;
	_textTexCoords[7] = 1.0f;

	glGenVertexArrays(1, &_textVAO);
	glGenBuffers(1, &_textVBO);
	glGenBuffers(1, &_textIBO);
	glGenBuffers(1, &_textTBO);

	glBindVertexArray(_textVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _textVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, _textVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _textIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, _textIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _textTBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, _textTexCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageUIText::CreateText(std::string filename, int size, int dpi)
{
	_glyphs = new GLuint[128];
	_boundingBoxes = new FT_BBox[128];
	_glyphWidths = new int[128];
	_glyphHeights = new int[128];
	_fontHeight = size;

	FT_Library library;
	FT_Init_FreeType(&library);
	FT_Face face;
	FT_New_Face(library, filename.c_str(), 0, &face);
	FT_Stroker stroker;
	FT_Stroker_New(library, &stroker);
	FT_Stroker_Set(stroker, 1*64, FT_STROKER_LINECAP_SQUARE, FT_STROKER_LINEJOIN_BEVEL, 0);
	FT_Set_Char_Size(face, _fontHeight*64, _fontHeight*64, dpi, dpi);

	glGenTextures(128, _glyphs);

	for (int i = 0;i < 128;i ++)
	{
		CreateGlyph(face, stroker, (char)i, _glyphs);
	}

	FT_Stroker_Done(stroker);
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return 0;
}

glm::mat4 Phage::PhageUIText::GetMatrix()
{
	return _matrix.GetMatrix(0);
}

int Phage::PhageUIText::AddUITextIcon(char c, unsigned int texture)
{
	_textIconVector.push_back(Phage::PhageUIText::TextIcon());
	_textIconVector.back().c = c;
	_textIconVector.back().tex = texture;

	return 0;
}

int Phage::PhageUIText::Draw(int x, int y, std::string str)
{
	int xOffset = 0;
	glBindVertexArray(_textVAO);
	
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0;i < (int)str.size();i ++)
	{
		if (str[i] == ' ')
		{
			xOffset += _fontHeight/2;
			continue;
		}
		else if (str[i] == '^')
		{
			unsigned int resourceTex;
			if ((int)str.size() > i+1)
			{
				for (int k = 0;k < (int)_textIconVector.size();k ++)
				{
					if (str[i+1] == _textIconVector[k].c)
					{
						resourceTex = _textIconVector[k].tex;
						break;
					}
				}
			}
			else
			{
				continue;
			}

			int hM = (5*_fontHeight)/4;

			_textVertices[0] = (float)(x+xOffset);
			_textVertices[1] = (float)(y);
			_textVertices[3] = (float)(x+xOffset+(4*hM)/3);
			_textVertices[4] = (float)(y);
			_textVertices[6] = (float)(x+xOffset+(4*hM)/3);
			_textVertices[7] = (float)(y+(hM));
			_textVertices[9] = (float)(x+xOffset);
			_textVertices[10] = (float)(y+(hM));
			glBindBuffer(GL_ARRAY_BUFFER, _textVBO);
			glBufferData(GL_ARRAY_BUFFER, 12*4, _textVertices, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, resourceTex);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			xOffset += (4*hM)/3;
			i++;
			continue;
		}
		_textVertices[0] = (float)(x+_boundingBoxes[str[i]].xMin+xOffset);
		_textVertices[1] = (float)(y+(_fontHeight-_boundingBoxes[str[i]].yMax));
		_textVertices[3] = (float)(x+_boundingBoxes[str[i]].xMax+xOffset);
		_textVertices[4] = (float)(y+(_fontHeight-_boundingBoxes[str[i]].yMax));
		_textVertices[6] = (float)(x+_boundingBoxes[str[i]].xMax+xOffset);
		_textVertices[7] = (float)(y+(_fontHeight-_boundingBoxes[str[i]].yMin));
		_textVertices[9] = (float)(x+_boundingBoxes[str[i]].xMin+xOffset);
		_textVertices[10] = (float)(y+(_fontHeight-_boundingBoxes[str[i]].yMin));
		glBindBuffer(GL_ARRAY_BUFFER, _textVBO);
		glBufferData(GL_ARRAY_BUFFER, 12*4, _textVertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, _glyphs[str[i]]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		xOffset += _boundingBoxes[str[i]].xMax-_boundingBoxes[str[i]].xMin;
		xOffset += _fontHeight/8;
	}
	glDisable(GL_BLEND);

	glBindVertexArray(0);

	return 0;
}

int Phage::PhageUIText::CreateGlyph(FT_Face face, FT_Stroker stroker, char c, GLuint* tex)
{
	FT_Load_Glyph(face, FT_Get_Char_Index(face, c), FT_LOAD_DEFAULT);
	FT_Glyph glyph;
	FT_Get_Glyph(face->glyph, &glyph);
	FT_Glyph_StrokeBorder(&glyph, stroker, false, true);
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, true);
	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmapGlyph->bitmap;

	FT_BBox box;
	FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_UNSCALED, &box);
	box.xMin /= 64;
	box.yMin /= 64;
	box.xMax /= 64;
	box.yMax /= 64;
	_boundingBoxes[c] = box;

	int width = bitmap.width;
	_glyphWidths[c] = bitmap.width;
	int height = bitmap.rows;
	_glyphHeights[c] = bitmap.rows;

	GLubyte* data = new GLubyte[width*height*4];

	for (int j = 0;j < height;j ++)
	{
		for (int i = 0;i < width;i ++)
		{
			data[(i+j*width)*4] = 0;
			data[(i+j*width)*4+1] = 0;
			data[(i+j*width)*4+2] = 0;
			data[(i+j*width)*4+3] = bitmap.buffer[i + bitmap.width*j];
		}
	}

	FT_Glyph glyphInside;
	FT_Get_Glyph(face->glyph, &glyphInside);
	FT_Glyph_To_Bitmap(&glyphInside, FT_RENDER_MODE_NORMAL, 0, true);
	FT_BitmapGlyph bitmapGlyphInside = (FT_BitmapGlyph)glyphInside;
	FT_Bitmap& bitmapInside = bitmapGlyphInside->bitmap;

	int widthInside = bitmapInside.width;
	int heightInside = bitmapInside.rows;

	int wo = (width - widthInside) / 2;
	int ho = (height - heightInside) / 2;

	for (int j = 0;j < heightInside;j ++)
	{
		for (int i = 0;i < widthInside;i ++)
		{
			data[((i+wo)+(j+ho)*width)*4] = bitmapInside.buffer[i + widthInside*j];
			data[((i+wo)+(j+ho)*width)*4+1] = bitmapInside.buffer[i + widthInside*j];
			data[((i+wo)+(j+ho)*width)*4+2] = bitmapInside.buffer[i + widthInside*j];
			//data[((i+wo)+(j+ho)*width)*4+3 = bitmapInside.buffer[i + widthInside*j];
		}
	}

	glBindTexture(GL_TEXTURE_2D, tex[c]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete[] data;
	FT_Done_Glyph(glyph);
	FT_Done_Glyph(glyphInside);

	return 0;
}
