#include "Phage_Line.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageLine::PhageLine()
{
	_vertices = NULL;
}

Phage::PhageLine::~PhageLine()
{
	delete[] _vertices;
}

int Phage::PhageLine::Init()
{
	_vertices = new float[6];

	_vertices[0] = 0.0f;
	_vertices[1] = 0.0f;
	_vertices[2] = 0.0f;

	_vertices[3] = 0.0f;
	_vertices[4] = 0.0f;
	_vertices[5] = 0.0f;

	_indices = new GLuint[2];

	_indices[0] = 0;
	_indices[1] = 1;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 6*4, _vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2*4, _indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageLine::SetPoints(glm::vec3 pt1, glm::vec3 pt2)
{
	_vertices[0] = pt1.x;
	_vertices[1] = pt1.y;
	_vertices[2] = pt1.z;

	_vertices[3] = pt2.x;
	_vertices[4] = pt2.y;
	_vertices[5] = pt2.z;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 6*4, _vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

glm::mat4 Phage::PhageLine::GetMatrix()
{
	return _matrix.GetMatrix(1);
}

int Phage::PhageLine::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}
