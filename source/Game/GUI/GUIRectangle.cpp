#include "GUIRectangle.h"

GUIRectangle::GUIRectangle()
{
	guiRectangleVertices = new float[12];

	guiRectangleVertices[0] = 10.0f;
	guiRectangleVertices[1] = 10.0f;
	guiRectangleVertices[2] = OrthoDepth(ORTHO_RECTANGLE);

	guiRectangleVertices[3] = 50.0f;
	guiRectangleVertices[4] = 10.0f;
	guiRectangleVertices[5] = OrthoDepth(ORTHO_RECTANGLE);

	guiRectangleVertices[6] = 50.0f;
	guiRectangleVertices[7] = 50.0f;
	guiRectangleVertices[8] = OrthoDepth(ORTHO_RECTANGLE);

	guiRectangleVertices[9] = 10.0f;
	guiRectangleVertices[10] = 50.0f;
	guiRectangleVertices[11] = OrthoDepth(ORTHO_RECTANGLE);

	guiRectangleIndices = new GLuint[6];

	guiRectangleIndices[0] = 0;
	guiRectangleIndices[1] = 2;
	guiRectangleIndices[2] = 1;
	guiRectangleIndices[3] = 0;
	guiRectangleIndices[4] = 3;
	guiRectangleIndices[5] = 2;

	glGenVertexArrays(1, &guiRectangleVAO);
	glGenBuffers(1, &guiRectangleVBO);
	glGenBuffers(1, &guiRectangleIBO);

	glBindVertexArray(guiRectangleVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, guiRectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, guiRectangleVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, guiRectangleIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, guiRectangleIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GUIRectangle::~GUIRectangle()
{
}

int GUIRectangle::Draw()
{
	glBindVertexArray(guiRectangleVAO);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}

int GUIRectangle::SetPoints(glm::vec4 points)
{
	guiRectangleVertices[0] = points[0];
	guiRectangleVertices[1] = points[1];

	guiRectangleVertices[3] = points[2];
	guiRectangleVertices[4] = points[1];

	guiRectangleVertices[6] = points[2];
	guiRectangleVertices[7] = points[3];

	guiRectangleVertices[9] = points[0];
	guiRectangleVertices[10] = points[3];

	glBindBuffer(GL_ARRAY_BUFFER, guiRectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, guiRectangleVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int GUIRectangle::SetDepth(int depth)
{
	float dep = (float)depth;
	if (dep < 0.0f) dep = 0.0f;
	if (dep > 99.0f) dep = 99.0f;

	guiRectangleVertices[2] = OrthoDepth(ORTHO_RECTANGLE) - dep/100000.0f;
	guiRectangleVertices[5] = OrthoDepth(ORTHO_RECTANGLE) - dep/100000.0f;
	guiRectangleVertices[8] = OrthoDepth(ORTHO_RECTANGLE) - dep/100000.0f;
	guiRectangleVertices[11] = OrthoDepth(ORTHO_RECTANGLE) - dep/100000.0f;

	glBindBuffer(GL_ARRAY_BUFFER, guiRectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, guiRectangleVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}
