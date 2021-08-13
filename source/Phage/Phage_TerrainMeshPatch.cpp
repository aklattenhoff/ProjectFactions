#include "Phage_TerrainMeshPatch.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageTerrainMeshPatch::PhageTerrainMeshPatch()
{
}

Phage::PhageTerrainMeshPatch::~PhageTerrainMeshPatch()
{
}

int Phage::PhageTerrainMeshPatch::Init()
{
	_width = 8;
	_height = 8;

	return 0;
}

int Phage::PhageTerrainMeshPatch::CreatePatch(int w, int h, std::vector<float> vertices, std::vector<float> textureCoords, std::vector<float> smoothNormals, std::vector<float> tangents, std::vector<float> alphaCoords)
{
	_width = w;
	_height = h;

	glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, (_width*_height*6*3)*4, &vertices.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_tbo);
    glBindBuffer(GL_ARRAY_BUFFER, _tbo);
    glBufferData(GL_ARRAY_BUFFER, (_width*_height*6*2)*4, &textureCoords.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, (_width*_height*6*3)*4, &smoothNormals.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_abo);
    glBindBuffer(GL_ARRAY_BUFFER, _abo);
    glBufferData(GL_ARRAY_BUFFER, (_width*_height*6*2)*4, &alphaCoords.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_tnbo);
    glBindBuffer(GL_ARRAY_BUFFER, _tnbo);
    glBufferData(GL_ARRAY_BUFFER, (_width*_height*6*3)*4, &tangents.front(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return 0;
}

int Phage::PhageTerrainMeshPatch::Render()
{
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _width*_height*6);
	glBindVertexArray(0);

	return 0;
}
