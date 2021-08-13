#ifndef _ENTITY_STATS_H
#define _ENTITY_STATS_H

#include "Game/Entity/EntityBase.h"

class EntityStats
{
public:
	EntityStats();
	~EntityStats();

	int SetEntityBase(EntityBase* entityBase);

	float MovementSpeed();
	float Radius();
	float AwarenessRadius();
private:
	EntityBase* _entityBase;
};

#endif
