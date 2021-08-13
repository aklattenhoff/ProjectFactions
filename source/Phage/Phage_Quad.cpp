#include "Phage_Quad.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageQuad::PhageQuad()
{
	_vertices = NULL;
	_indices = NULL;
	_texCoords = NULL;
}

Phage::PhageQuad::~PhageQuad()
{
	delete[] _vertices;
	delete[] _indices;
	delete[] _texCoords;
}

int Phage::PhageQuad::CreateTextureQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, std::string fileName, bool upsideDown)
{
	//Load texture from file
	SDL_Surface *surface = NULL;
	surface = IMG_Load(fileName.c_str());
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

	_matrix.Identity();
	//_matrix.SetTranslate(x, y, 0.0f);
	//_matrix.Scale(w < 1.0f ? surface->w : w, h < 1.0f ? surface->h : h, 1.0f);

	_vertices = new float[12];

	_vertices[0] = p1.x;
	_vertices[1] = p1.y;
	_vertices[2] = p1.z;

	_vertices[3] = p2.x;
	_vertices[4] = p2.y;
	_vertices[5] = p2.z;

	_vertices[6] = p3.x;
	_vertices[7] = p3.y;
	_vertices[8] = p3.z;

	_vertices[9] = p4.x;
	_vertices[10] = p4.y;
	_vertices[11] = p4.z;

	_indices = new GLuint[6];

	_indices[0] = 0;
	_indices[1] = 2;
	_indices[2] = 1;
	_indices[3] = 0;
	_indices[4] = 3;
	_indices[5] = 2;

	_texCoords = new float[8];

	_texCoords[0] = 0.0f;
	_texCoords[1] = 0.0f;

	_texCoords[2] = 1.0f;
	_texCoords[3] = 0.0f;

	_texCoords[4] = 1.0f;
	_texCoords[5] = 1.0f;

	_texCoords[6] = 0.0f;
	_texCoords[7] = 1.0f;

	if (upsideDown)
	{
		_texCoords[0] = 0.0f;
		_texCoords[1] = 1.0f;

		_texCoords[2] = 1.0f;
		_texCoords[3] = 1.0f;

		_texCoords[4] = 1.0f;
		_texCoords[5] = 0.0f;

		_texCoords[6] = 0.0f;
		_texCoords[7] = 0.0f;
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	glGenBuffers(1, &_tbo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * 4, _vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4, _indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4, _texCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageQuad::CreateTextureQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, GLuint textureID, bool upsideDown)
{
	_textureID = textureID;

	_matrix.Identity();
	//_matrix.SetTranslate(x, y, 0.0f);
	//_matrix.Scale(w, h, 1.0f);

	_vertices = new float[12];

	_vertices[0] = p1.x;
	_vertices[1] = p1.y;
	_vertices[2] = p1.z;

	_vertices[3] = p2.x;
	_vertices[4] = p2.y;
	_vertices[5] = p2.z;

	_vertices[6] = p3.x;
	_vertices[7] = p3.y;
	_vertices[8] = p3.z;

	_vertices[9] = p4.x;
	_vertices[10] = p4.y;
	_vertices[11] = p4.z;

	_indices = new GLuint[6];

	_indices[0] = 0;
	_indices[1] = 2;
	_indices[2] = 1;
	_indices[3] = 0;
	_indices[4] = 3;
	_indices[5] = 2;

	_texCoords = new float[8];

	_texCoords[0] = 0.0f;
	_texCoords[1] = 0.0f;

	_texCoords[2] = 1.0f;
	_texCoords[3] = 0.0f;

	_texCoords[4] = 1.0f;
	_texCoords[5] = 1.0f;

	_texCoords[6] = 0.0f;
	_texCoords[7] = 1.0f;

	if (upsideDown)
	{
		_texCoords[0] = 0.0f;
		_texCoords[1] = 1.0f;

		_texCoords[2] = 1.0f;
		_texCoords[3] = 1.0f;

		_texCoords[4] = 1.0f;
		_texCoords[5] = 0.0f;

		_texCoords[6] = 0.0f;
		_texCoords[7] = 0.0f;
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ibo);
	glGenBuffers(1, &_tbo);

	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * 4, _vertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 4, _indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4, _texCoords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}

int Phage::PhageQuad::SetTextureID(GLuint textureID)
{
	_textureID = textureID;

	return 0;
}

Phage::Matrix Phage::PhageQuad::GetMatrix()
{
	return _matrix;
}

int Phage::PhageQuad::Draw()
{
	//Draw Vertices
	glBindVertexArray(_vao);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}
