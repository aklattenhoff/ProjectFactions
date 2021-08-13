#include "GUIRectangle.h"

RS::GUIRectangle::GUIRectangle()
{

}

RS::GUIRectangle::~GUIRectangle()
{

}

int RS::GUIRectangle::SetPoints(int x1, int y1, int x2, int y2)
{
	_points[0] = (float)x1;
	_points[1] = (float)y1;
	_points[2] = (float)x2;
	_points[3] = (float)y2;

	return 0;
}

glm::vec4 RS::GUIRectangle::GetPoints()
{
	return _points;
}
