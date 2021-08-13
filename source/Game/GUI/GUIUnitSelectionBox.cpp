#include "GUIUnitSelectionBox.h"

GUIUnitSelectionBox::GUIUnitSelectionBox()
{
	selectionBoxVertices = new float[12];

	selectionBoxVertices[0] = 10.0f;
	selectionBoxVertices[1] = 10.0f;
	selectionBoxVertices[2] = OrthoDepth(ORTHO_SELECTION_BOX);

	selectionBoxVertices[3] = 50.0f;
	selectionBoxVertices[4] = 10.0f;
	selectionBoxVertices[5] = OrthoDepth(ORTHO_SELECTION_BOX);

	selectionBoxVertices[6] = 50.0f;
	selectionBoxVertices[7] = 50.0f;
	selectionBoxVertices[8] = OrthoDepth(ORTHO_SELECTION_BOX);

	selectionBoxVertices[9] = 10.0f;
	selectionBoxVertices[10] = 50.0f;
	selectionBoxVertices[11] = OrthoDepth(ORTHO_SELECTION_BOX);

	selectionBoxIndices = new GLuint[8];

	selectionBoxIndices[0] = 0;
	selectionBoxIndices[1] = 1;
	selectionBoxIndices[2] = 1;
	selectionBoxIndices[3] = 2;
	selectionBoxIndices[4] = 2;
	selectionBoxIndices[5] = 3;
	selectionBoxIndices[6] = 3;
	selectionBoxIndices[7] = 0;

	glGenVertexArrays(1, &selectionBoxVAO);
	glGenBuffers(1, &selectionBoxVBO);
	glGenBuffers(1, &selectionBoxIBO);

	glBindVertexArray(selectionBoxVAO);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, selectionBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*4, selectionBoxVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectionBoxIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*4, selectionBoxIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GUIUnitSelectionBox::~GUIUnitSelectionBox()
{
}

int GUIUnitSelectionBox::Draw()
{
	glBindVertexArray(selectionBoxVAO);
	
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	return 0;
}

int GUIUnitSelectionBox::SetPoints(glm::vec4 points)
{
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

	return 0;
}
