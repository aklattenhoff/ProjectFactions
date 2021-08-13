#include "GUITexture.h"

RS::GUITexture::GUITexture()
{

}

RS::GUITexture::~GUITexture()
{

}

int RS::GUITexture::SetPoints(int x1, int y1, int x2, int y2, float depth)
{
	_points[0] = (float)x1;
	_points[1] = (float)y1;
	_points[2] = (float)x2;
	_points[3] = (float)y2;

	_depth = depth;

	return 0;
}

int RS::GUITexture::SetTexture(unsigned int tex)
{
	_texture = tex;

	return 0;
}

glm::vec4 RS::GUITexture::GetPoints()
{
	return _points;
}

float RS::GUITexture::GetDepth()
{
	return _depth;
}

unsigned int RS::GUITexture::GetTexture()
{
	return _texture;
}
