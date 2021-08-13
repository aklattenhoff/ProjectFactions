#include "Phage_UILineRect.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageUILineRect::PhageUILineRect()
{
	selectionBoxVertices = NULL;
	selectionBoxIndices = NULL;
}

Phage::PhageUILineRect::~PhageUILineRect()
{
	delete[] selectionBoxVertices;
	delete[] selectionBoxIndices;
}

int Phage::PhageUILineRect::CreateLineRect()
{
	//Load texture from file
	SDL_Surface *surface = NULL;
	surface = IMG_Load("textures/blank.png");
	if (!surface) return -1;

	GLenum textureFormat;
	if (surface->format->BytesPerPixel == 4)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	selectionBoxVertices = new float[12];

	selectionBoxVertices[0] = 10.0f;
	selectionBoxVertices[1] = 10.0f;
	selectionBoxVertices[2] = 0.0f;

	selectionBoxVertices[3] = 50.0f;
	selectionBoxVertices[4] = 10.0f;
	selectionBoxVertices[5] = 0.0f;

	selectionBoxVertices[6] = 50.0f;
	selectionBoxVertices[7] = 50.0f;
	selectionBoxVertices[8] = 0.0f;

	selectionBoxVertices[9] = 10.0f;
	selectionBoxVertices[10] = 50.0f;
	selectionBoxVertices[11] = 0.0f;

	selectionBoxIndices = new GLuint[8];

	selectionBoxIndices[0] = 0;
	selectionBoxIndices[1] = 1;
	selectionBoxIndices[2] = 1;
	selectionBoxIndices[3] = 2;
	selectionBoxIndices[4] = 2;
	selectionBoxIndices[5] = 3;
	selectionBoxIndices[6] = 3;
	selectionBoxIndices[7] = 0;

	selectionBoxTexCoords = new float[8];

	selectionBoxTexCoords[0] = 0.0f;
	selectionBoxTexCoords[1] = 0.0f;

	selectionBoxTexCoords[2] = 1.0f;
	selectionBoxTexCoords[3] = 0.0f;

	selectionBoxTexCoords[4] = 1.0f;
	selectionBoxTexCoords[5] = 1.0f;

	selectionBoxTexCoords[6] = 0.0f;
	selectionBoxTexCoords[7] = 1.0f;

	glGenVertexArrays(1, &selectionBoxVAO);
	glGenBuffers(1, &selectionBoxVBO);
	glGenBuffers(1, &selectionBoxIBO);
	glGenBuffers(1, &selectionBoxTBO);

	glBindVertexArray(selectionBoxVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, selectionBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, selectionBoxVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectionBoxIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, selectionBoxIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, selectionBoxTBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, selectionBoxTexCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageUILineRect::Draw(glm::vec4 points)
{
	glBindVertexArray(selectionBoxVAO);

	selectionBoxVertices[0] = points[0];
	selectionBoxVertices[1] = points[1];

	selectionBoxVertices[3] = points[2];
	selectionBoxVertices[4] = points[1];

	selectionBoxVertices[6] = points[2];
	selectionBoxVertices[7] = points[3];

	selectionBoxVertices[9] = points[0];
	selectionBoxVertices[10] = points[3];

	glBindBuffer(GL_ARRAY_BUFFER, selectionBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, selectionBoxVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}
