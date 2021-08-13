#include "GUITexture.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

GUITexture::GUITexture()
{
	guiTextureVertices = new float[12];

	guiTextureVertices[0] = 10.0f;
	guiTextureVertices[1] = 10.0f;
	guiTextureVertices[2] = 0.3f;

	guiTextureVertices[3] = 50.0f;
	guiTextureVertices[4] = 10.0f;
	guiTextureVertices[5] = 0.3f;

	guiTextureVertices[6] = 50.0f;
	guiTextureVertices[7] = 50.0f;
	guiTextureVertices[8] = 0.3f;

	guiTextureVertices[9] = 10.0f;
	guiTextureVertices[10] = 50.0f;
	guiTextureVertices[11] = 0.3f;

	guiTextureIndices = new GLuint[6];

	guiTextureIndices[0] = 0;
	guiTextureIndices[1] = 2;
	guiTextureIndices[2] = 1;
	guiTextureIndices[3] = 0;
	guiTextureIndices[4] = 3;
	guiTextureIndices[5] = 2;

	guiTextureTexCoords = new float[8];

	guiTextureTexCoords[0] = 0.0f;
	guiTextureTexCoords[1] = 0.0f;

	guiTextureTexCoords[2] = 1.0f;
	guiTextureTexCoords[3] = 0.0f;

	guiTextureTexCoords[4] = 1.0f;
	guiTextureTexCoords[5] = 1.0f;

	guiTextureTexCoords[6] = 0.0f;
	guiTextureTexCoords[7] = 1.0f;

	glGenVertexArrays(1, &guiTextureVAO);
	glGenBuffers(1, &guiTextureVBO);
	glGenBuffers(1, &guiTextureIBO);

	glBindVertexArray(guiTextureVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, guiTextureVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, guiTextureVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, guiTextureIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, guiTextureIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &guiTextureTBO);
    glBindBuffer(GL_ARRAY_BUFFER, guiTextureTBO);
    glBufferData(GL_ARRAY_BUFFER, 8*4, guiTextureTexCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GUITexture::~GUITexture()
{
}

int GUITexture::Draw(GLuint tex)
{
	glBindVertexArray(guiTextureVAO);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}

int GUITexture::SetPoints(glm::vec4 points, float depth)
{
	guiTextureVertices[0] = points[0];
	guiTextureVertices[1] = points[1];
	guiTextureVertices[2] = depth;

	guiTextureVertices[3] = points[2];
	guiTextureVertices[4] = points[1];
	guiTextureVertices[5] = depth;

	guiTextureVertices[6] = points[2];
	guiTextureVertices[7] = points[3];
	guiTextureVertices[8] = depth;

	guiTextureVertices[9] = points[0];
	guiTextureVertices[10] = points[3];
	guiTextureVertices[11] = depth;

	glBindBuffer(GL_ARRAY_BUFFER, guiTextureVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, guiTextureVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}
