#include "Line2D.h"

Line2D::Line2D()
{
}

Line2D::~Line2D()
{
}

int Line2D::CreateLineFromTwoPoints(float x1, float y1, float x2, float y2)
{
	point1 = glm::vec2(x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2);
	point2 = glm::vec2(x1 > x2 ? x1 : x2, y1 > y2 ? y1 : y2);

	if ((x1-x2) == 0.0f)
		m = (y1-y2)/0.0001f;
	else
		m = (y1-y2)/(x1-x2);
	b = y1-m*x1;

	return 0;
}

int Line2D::SetLineSegment(float x1, float y1, float x2, float y2)
{
	point1 = glm::vec2(x1 < x2 ? x1 : x2, y1 < y2 ? y1 : y2);
	point2 = glm::vec2(x1 > x2 ? x1 : x2, y1 > y2 ? y1 : y2);

	return 0;
}

float Line2D::CalculateX(float y)
{
	if (m == 0.0f)
		return (y-b)/(m+0.0001f);
	else
		return (y-b)/m;
}

float Line2D::CalculateY(float x)
{
	return m*x+b;
}

bool Line2D::CalculateXFromSegment(float y, float* returnX)
{
	if (y < point1.y || y > point2.y)
		return false;
	else
	{
		if (m == 0.0f)
			(*returnX) =  (y-b)/(m+0.0001f);
		else
			(*returnX) = (y-b)/m;
		return true;
	}
}

bool Line2D::CalculateYFromSegment(float x, float* returnY)
{
	if (x < point1.x || x > point2.x)
		return false;
	else
	{
		(*returnY) =  m*x+b;
		return true;
	}
}
