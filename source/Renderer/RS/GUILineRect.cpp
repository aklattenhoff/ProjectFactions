#include "GUILineRect.h"

RS::GUILineRect::GUILineRect()
{

}

RS::GUILineRect::~GUILineRect()
{

}

int RS::GUILineRect::SetPoints(int x1, int y1, int x2, int y2)
{
	_points[0] = (float)x1;
	_points[1] = (float)y1;
	_points[2] = (float)x2;
	_points[3] = (float)y2;

	return 0;
}

int RS::GUILineRect::SetPoints(glm::vec4 ps)
{
	_points = ps;

	return 0;
}

glm::vec4 RS::GUILineRect::GetPoints()
{
	return _points;
}
