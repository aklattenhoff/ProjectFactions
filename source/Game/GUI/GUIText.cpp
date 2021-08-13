#include "GUIText.h"

GUIText::GUIText()
{
	textVertices = new float[12];

	textVertices[0] = 0.0f;
	textVertices[1] = 0.0f;
	textVertices[2] = OrthoDepth(ORTHO_TEXT);

	textVertices[3] = 10.0f;
	textVertices[4] = 0.0f;
	textVertices[5] = OrthoDepth(ORTHO_TEXT);

	textVertices[6] = 10.0f;
	textVertices[7] = 10.0f;
	textVertices[8] = OrthoDepth(ORTHO_TEXT);

	textVertices[9] = 0.0f;
	textVertices[10] = 10.0f;
	textVertices[11] = OrthoDepth(ORTHO_TEXT);

	textIndices = new GLuint[6];

	textIndices[0] = 0;
	textIndices[1] = 2;
	textIndices[2] = 1;
	textIndices[3] = 0;
	textIndices[4] = 3;
	textIndices[5] = 2;

	textTexCoords = new float[8];

	textTexCoords[0] = 0.0f;
	textTexCoords[1] = 0.0f;

	textTexCoords[2] = 1.0f;
	textTexCoords[3] = 0.0f;

	textTexCoords[4] = 1.0f;
	textTexCoords[5] = 1.0f;

	textTexCoords[6] = 0.0f;
	textTexCoords[7] = 1.0f;

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glGenBuffers(1, &textIBO);

	glBindVertexArray(textVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, textVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, textIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &textTBO);
    glBindBuffer(GL_ARRAY_BUFFER, textTBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, textTexCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glyphs = NULL;
	boundingBoxes = NULL;
	glyphWidths = NULL;
	glyphHeights = NULL;
}

GUIText::~GUIText()
{
	delete[] textVertices;
	delete[] textIndices;
	delete[] textTexCoords;
	delete[] glyphs;
	delete[] boundingBoxes;
	delete[] glyphWidths;
	delete[] glyphHeights;
}

int GUIText::CreateGlyph(FT_Face face, FT_Stroker stroker, char c, GLuint* tex)
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
	boundingBoxes[c] = box;

	int width = bitmap.width;
	glyphWidths[c] = bitmap.width;
	int height = bitmap.rows;
	glyphHeights[c] = bitmap.rows;

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

int GUIText::Init(char* filename, int size, int dpi)
{
	glyphs = new GLuint[128];
	boundingBoxes = new FT_BBox[128];
	glyphWidths = new int[128];
	glyphHeights = new int[128];
	fontHeight = size;

	FT_Library library;
	FT_Init_FreeType(&library);
	FT_Face face;
	FT_New_Face(library, filename, 0, &face);
	FT_Stroker stroker;
	FT_Stroker_New(library, &stroker);
	FT_Stroker_Set(stroker, 1*64, FT_STROKER_LINECAP_SQUARE, FT_STROKER_LINEJOIN_BEVEL, 0);
	FT_Set_Char_Size(face, fontHeight*64, fontHeight*64, dpi, dpi);

	glGenTextures(128, glyphs);

	for (int i = 0;i < 128;i ++)
	{
		CreateGlyph(face, stroker, (char)i, glyphs);
	}

	FT_Stroker_Done(stroker);
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return 0;
}

/*int GUIText::Draw(int x, int y, std::string str, Camera* camera, std::vector<unsigned int>* resourceTexVector)
{
	int xOffset = 0;
	glBindVertexArray(textVAO);
	
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0;i < (int)str.size();i ++)
	{
		if (str[i] == ' ')
		{
			xOffset += fontHeight/2;
			continue;
		}
		else if (str[i] == '^' && resourceTexVector)
		{
			unsigned int resourceTex;
			if (str.size() > i+1)
			{
				if (str[i+1] == 'f')
				{
					resourceTex = resourceTexVector->at(0);
				}
				else if (str[i+1] == 'w')
				{
					resourceTex = resourceTexVector->at(1);
				}
			}
			else
			{
				continue;
			}

			int hM = (5*fontHeight)/4;

			textVertices[0] = (float)(x+xOffset);
			textVertices[1] = (float)(y);
			textVertices[3] = (float)(x+xOffset+(4*hM)/3);
			textVertices[4] = (float)(y);
			textVertices[6] = (float)(x+xOffset+(4*hM)/3);
			textVertices[7] = (float)(y+(hM));
			textVertices[9] = (float)(x+xOffset);
			textVertices[10] = (float)(y+(hM));
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferData(GL_ARRAY_BUFFER, 12*4, textVertices, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, resourceTex);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			xOffset += (4*hM)/3;
			i++;
			continue;
		}
		textVertices[0] = (float)(x+boundingBoxes[str[i]].xMin+xOffset);
		textVertices[1] = (float)(y+(fontHeight-boundingBoxes[str[i]].yMax));
		textVertices[3] = (float)(x+boundingBoxes[str[i]].xMax+xOffset);
		textVertices[4] = (float)(y+(fontHeight-boundingBoxes[str[i]].yMax));
		textVertices[6] = (float)(x+boundingBoxes[str[i]].xMax+xOffset);
		textVertices[7] = (float)(y+(fontHeight-boundingBoxes[str[i]].yMin));
		textVertices[9] = (float)(x+boundingBoxes[str[i]].xMin+xOffset);
		textVertices[10] = (float)(y+(fontHeight-boundingBoxes[str[i]].yMin));
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, 12*4, textVertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, glyphs[str[i]]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		xOffset += boundingBoxes[str[i]].xMax-boundingBoxes[str[i]].xMin;
		xOffset += fontHeight/8;
	}
	glDisable(GL_BLEND);

	glBindVertexArray(0);

	return 0;
}*/

float GUIText::GetStringHeight()
{
	return (float)fontHeight;
}

glm::vec2 GUIText::CalcStringSize(std::string str)
{
	glm::vec2 size;
	size.x = 0.0f;
	size.y = (float)fontHeight;

	for (int i = 0;i < (int)str.size();i ++)
	{
		if (str[i] == ' ')
		{
			size.x += fontHeight/2;
		}
		size.x += (float)(boundingBoxes[str[i]].xMax-boundingBoxes[str[i]].xMin);
		size.x += (float)(fontHeight/8);
	}

	return size;
}

int GUIText::Destroy()
{
	glDeleteTextures(128, glyphs);

	return 0;
}
