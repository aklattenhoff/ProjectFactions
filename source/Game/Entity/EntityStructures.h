#ifndef _ENTITY_STRUCTURES_H
#define _ENTITY_STRUCTURES_H

namespace entityStructs
{
	struct vector2D
	{
		float direction;
		float magnitude;

		vector2D()
		{
			direction = 0.0f;
			magnitude = 0.0f;
		}
	};
};

#endif
