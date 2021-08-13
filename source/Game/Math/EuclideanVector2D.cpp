#include "EuclideanVector2D.h"

EuclideanVector2D::EuclideanVector2D()
{
	direction = 0.0f;
	magnitude = 0.0f;
}

EuclideanVector2D::EuclideanVector2D(float dir, float mag)
{
	direction = dir;
	magnitude = mag;
}

EuclideanVector2D::~EuclideanVector2D()
{
}
