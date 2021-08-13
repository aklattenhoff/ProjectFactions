#ifndef _EUCLIDEAN_VECTOR_2D_H
#define _EUCLIDEAN_VECTOR_2D_H

//Contains direction and magnitude of the vector
class EuclideanVector2D
{
public:
	EuclideanVector2D();
	EuclideanVector2D(float dir, float mag);
	~EuclideanVector2D();

	float direction;
	float magnitude;
};

#endif
