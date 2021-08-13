#include "Phage_TerrainTileDebug.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageTerrainTileDebug::PhageTerrainTileDebug()
{
	_vertices = NULL;
}

Phage::PhageTerrainTileDebug::~PhageTerrainTileDebug()
{
	delete[] _vertices;
}

int Phage::PhageTerrainTileDebug::Init()
{
	_vertices = new float[12];

	_vertices[0] = 0.0f;
	_vertices[1] = 0.0f;
	_vertices[2] = 0.0f;

	_vertices[3] = 0.5f;
	_vertices[4] = 0.0f;
	_vertices[5] = 0.0f;

	_vertices[6] = 0.5f;
	_vertices[7] = 0.0f;
	_vertices[8] = 0.5f;

	_vertices[9] = 0.0f;
	_vertices[10] = 0.0f;
	_vertices[11] = 0.5f;

	_indices = new GLuint[12];

	_indices[0] = 0;
	_indices[1] = 1;
	_indices[2] = 1;
	_indices[3] = 2;
	_indices[4] = 2;
	_indices[5] = 3;
	_indices[6] = 3;
	_indices[7] = 0;
	_indices[8] = 0;
	_indices[9] = 2;
	_indices[10] = 1;
	_indices[11] = 3;

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, 12*4, _vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*4, _indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageTerrainTileDebug::SetTranslate(float x, float y, float z)
{
	_matrix.SetTranslate(x, y, z);

	return 0;
}

glm::mat4 Phage::PhageTerrainTileDebug::GetMatrix()
{
	return _matrix.GetMatrix(1);
}

int Phage::PhageTerrainTileDebug::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}
