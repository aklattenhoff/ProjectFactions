#include "TerrainDecal.h"

#include "Debug/Debug.h"

RS::TerrainDecal::TerrainDecal()
{
	_bumpTexture = -1;
	_rotation = 0.0f;
}

RS::TerrainDecal::~TerrainDecal()
{

}

int RS::TerrainDecal::SetPoints(float x, float y, float w, float h, float depth)
{
	_points[0] = x;
	_points[1] = y;
	_points[2] = w;
	_points[3] = h;

	_depth = depth;

	return 0;
}

int RS::TerrainDecal::SetOrigin(float x, float y)
{
	_origin.x = x;
	_origin.y = y;

	return 0;
}

int RS::TerrainDecal::SetRotation(float rot)
{
	_rotation = rot;

	return 0;
}

int RS::TerrainDecal::SetTexture(unsigned int tex)
{
	_texture = tex;

	return 0;
}

int RS::TerrainDecal::SetBumpTexture(unsigned int tex)
{
	_bumpTexture = tex;

	return 0;
}

glm::vec4 RS::TerrainDecal::GetPoints()
{
	return _points;
}

glm::vec2 RS::TerrainDecal::GetOrigin()
{
	return _origin;
}

float RS::TerrainDecal::GetRotation()
{
	return _rotation;
}

float RS::TerrainDecal::GetDepth()
{
	return _depth;
}

unsigned int RS::TerrainDecal::GetTexture()
{
	return _texture;
}

unsigned int RS::TerrainDecal::GetBumpTexture()
{
	return _bumpTexture;
}
