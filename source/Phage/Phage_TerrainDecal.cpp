#include "Phage_TerrainDecal.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

Phage::PhageTerrainDecal::PhageTerrainDecal()
{
	_vertices = NULL;
	_texCoords = NULL;
}

Phage::PhageTerrainDecal::~PhageTerrainDecal()
{
	delete[] _vertices;
	delete[] _texCoords;
}

int Phage::PhageTerrainDecal::Init()
{
	return 0;
}

int Phage::PhageTerrainDecal::CreateTerrainDecal(std::string fileName, float x, float y, float rotation, float dpi, float w, float h)
{
	//Load texture from file
	SDL_Surface* surface = NULL;
	IMG_Init(IMG_INIT_PNG);
	surface = IMG_Load(fileName.c_str());

	if (!surface)
		return -1;

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	_centerPoint = glm::vec2(x, y);
	_size.x = w > 0.0f ? w : surface->w / dpi;
	_size.y = h > 0.0f ? h : surface->h / dpi;
	//_size = glm::vec2(surface->w / dpi, surface->h / dpi);
	_rotationInRadians = glm::radians(rotation);

	_rotatedPoint[0] = glm::rotate(glm::vec2(-_size.x / 2.0f, -_size.y / 2.0f), _rotationInRadians) + _centerPoint;
	_rotatedPoint[1] = glm::rotate(glm::vec2(_size.x / 2.0f, -_size.y / 2.0f), _rotationInRadians) + _centerPoint;
	_rotatedPoint[2] = glm::rotate(glm::vec2(_size.x / 2.0f, _size.y / 2.0f), _rotationInRadians) + _centerPoint;
	_rotatedPoint[3] = glm::rotate(glm::vec2(-_size.x / 2.0f, _size.y / 2.0f), _rotationInRadians) + _centerPoint;

	float left = _rotatedPoint[0].x;
	float right = _rotatedPoint[0].x;
	float top = _rotatedPoint[0].y;
	float bottom = _rotatedPoint[0].y;

	for (int i = 1;i < 4;i ++)
	{
		if (_rotatedPoint[i].x < left) left = _rotatedPoint[i].x;
		if (_rotatedPoint[i].x > right) right = _rotatedPoint[i].x;
		if (_rotatedPoint[i].y < top) top = _rotatedPoint[i].y;
		if (_rotatedPoint[i].y > bottom) bottom = _rotatedPoint[i].y;
	}

	_aaBoxPoint[0].x = left;
	_aaBoxPoint[1].x = right;
	_aaBoxPoint[0].y = top;
	_aaBoxPoint[1].y = bottom;

	_gridPoint.x = (int)_aaBoxPoint[0].x;
	_gridPoint.y = (int)_aaBoxPoint[0].y;
	_gridPoint.z = (int)glm::ceil(_aaBoxPoint[1].x);
	_gridPoint.w = (int)glm::ceil(_aaBoxPoint[1].y);

	return 0;
}

int Phage::PhageTerrainDecal::SetVertexData(std::vector<float>* vertexData, std::vector<float>* normalData, std::vector<float>* tangentData)
{
	_vertices = new float[vertexData->size()];
	for (int i = 0;i < (int)vertexData->size();i ++)
		_vertices[i] = vertexData->at(i);

	_normals = new float[normalData->size()];
	for (int i = 0;i < (int)normalData->size();i ++)
		_normals[i] = normalData->at(i);

	_tangents = new float[tangentData->size()];
	for (int i = 0;i < (int)tangentData->size();i ++)
		_tangents[i] = tangentData->at(i);

	//for (int i = 1;i < vertexData->size();i += 3)
		//_vertices[i] += 0.025f;

	_numTris = vertexData->size() / 9;
	int width = _gridPoint.z - _gridPoint.x;
	int height = _gridPoint.w - _gridPoint.y;

	_texCoords = new float[_numTris * 6];
	for (int j = _gridPoint.y; j < _gridPoint.w; j++)
		for (int i = _gridPoint.x; i < _gridPoint.z; i++)
		{
			int y = j - _gridPoint.y;
			int x = i - _gridPoint.x;
			glm::vec2 rot = glm::rotate(glm::vec2(i, j) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 0] = rot.x;
			_texCoords[(y*width + x) * 12 + 1] = rot.y;
			rot = glm::rotate(glm::vec2(i+1, j+1) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 2] = rot.x;
			_texCoords[(y*width + x) * 12 + 3] = rot.y;
			rot = glm::rotate(glm::vec2(i+1, j) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 4] = rot.x;
			_texCoords[(y*width + x) * 12 + 5] = rot.y;

			rot = glm::rotate(glm::vec2(i, j) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 6] = rot.x;
			_texCoords[(y*width + x) * 12 + 7] = rot.y;
			rot = glm::rotate(glm::vec2(i, j+1) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 8] = rot.x;
			_texCoords[(y*width + x) * 12 + 9] = rot.y;
			rot = glm::rotate(glm::vec2(i+1, j+1) - _centerPoint, _rotationInRadians) / _size + glm::vec2(0.5f, 0.5f);
			_texCoords[(y*width + x) * 12 + 10] = rot.x;
			_texCoords[(y*width + x) * 12 + 11] = rot.y;
		}

	glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, (_numTris*3*3)*4, _vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_tbo);
    glBindBuffer(GL_ARRAY_BUFFER, _tbo);
    glBufferData(GL_ARRAY_BUFFER, (_numTris*3*2)*4, _texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, (_numTris*3*3)*4, _normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glGenBuffers(1, &_tnbo);
    glBindBuffer(GL_ARRAY_BUFFER, _tnbo);
    glBufferData(GL_ARRAY_BUFFER, (_numTris*3*3)*4, _tangents, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return 0;
}

glm::ivec4 Phage::PhageTerrainDecal::GetGridPoints()
{
	return _gridPoint;
}

int Phage::PhageTerrainDecal::Draw()
{
	glBindVertexArray(_vao);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glDrawArrays(GL_TRIANGLES, 0, _numTris*3);
	glBindVertexArray(0);

	return 0;
}
