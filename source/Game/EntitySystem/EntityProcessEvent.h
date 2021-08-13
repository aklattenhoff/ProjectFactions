#ifndef _ENTITY_PROCESS_EVENT_H
#define _ENTITY_PROCESS_EVENT_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Path.h"

class EntityBase;
class Entity;
class Squadron;
class Group;

class EntityProcessEvent
{
public:
	static const int TRAIN_ENTITY = 0;
public:
	EntityProcessEvent();
	~EntityProcessEvent();

	int eventType;
	bool initialized;
	Entity* owner;

	EntityBase* entityBase;
	float trainPoints;
	float trainPointsRemaining;
	float debugTrainPoints;
	unsigned int timer;
private:
};

#endif
