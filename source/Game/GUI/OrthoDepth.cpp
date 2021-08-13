#include "OrthoDepth.h"

extern inline float OrthoDepth(enum OrthoLayer orthoLayer)
{
	return 1.0f - (float)(orthoLayer) / 1000.0f;
}
