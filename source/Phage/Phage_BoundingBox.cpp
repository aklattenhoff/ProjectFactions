#include "Phage_BoundingBox.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageBoundingBox::PhageBoundingBox()
{
	_vertices = NULL;
}

Phage::PhageBoundingBox::~PhageBoundingBox()
{
	delete[] _vertices;
}

int Phage::PhageBoundingBox::Init()
{
	_vertices = new float[24];

	_vertices[0] = -0.5f;
	_vertices[1] = 0.5f;
	_vertices[2] = -0.5f;

	_vertices[3] = 0.5f;
	_vertices[4] = 0.5f;
	_vertices[5] = -0.5f;

	_vertices[6] = 0.5f;
	_vertices[7] = 0.5f;
	_vertices[8] = 0.5f;

	_vertices[9] = -0.5f;
	_vertices[10] = 0.5f;
	_vertices[11] = 0.5f;

	_vertices[12] = -0.5f;
	_vertices[13] = -0.5f;
	_vertices[14] = -0.5f;

	_vertices[15] = 0.5f;
	_vertices[16] = -0.5f;
	_vertices[17] = -0.5f;

	_vertices[18] = 0.5f;
	_vertices[19] = -0.5f;
	_vertices[20] = 0.5f;

	_vertices[21] = -0.5f;
	_vertices[22] = -0.5f;
	_vertices[23] = 0.5f;

	_indices = new GLuint[24];

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 1;
	_indices[3] = 2;
	_indices[4] = 2;
	_indices[5] = 3;
	_indices[6] = 3;
	_indices[7] = 0;

	_indices[8] = 4;
	_indices[9] = 5;
	_indices[10] = 5;
	_indices[11] = 6;
	_indices[12] = 6;
	_indices[13] = 7;
	_indices[14] = 7;
	_indices[15] = 4;

	_indices[16] = 0;
	_indices[17] = 4;
	_indices[18] = 1;
	_indices[19] = 5;
	_indices[20] = 2;
	_indices[21] = 6;
	_indices[22] = 3;
	_indices[23] = 7;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 24*4, _vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24*4, _indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageBoundingBox::SetTranslate(float x, float y, float z)
{
	_matrix.SetTranslate(x, y, z);

	return 0;
}

int Phage::PhageBoundingBox::SetScale(float x, float y, float z)
{
	_matrix.SetScale(x, y, z);

	return 0;
}

int Phage::PhageBoundingBox::SetRotate(float a)
{
	_matrix.SetRotate(a, 0.0f, 1.0f, 0.0f);

	return 0;
}

int Phage::PhageBoundingBox::Translate(float x, float y, float z)
{
	_matrix.Translate(x, y, z);

	return 0;
}

int Phage::PhageBoundingBox::Rotate(float a, float x, float z, float y)
{
	_matrix.Rotate(a, x, y, z);

	return 0;
}

glm::mat4 Phage::PhageBoundingBox::GetMatrix()
{
	return _matrix.GetMatrix(1);
}

int Phage::PhageBoundingBox::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}
