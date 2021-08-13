#ifndef _ENTITY_EVENT_H
#define _ENTITY_EVENT_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Path.h"

class Entity;
class Squadron;
class Group;

class EntityEvent
{
public:
	static const int MOVE_TO_POSITION = 1;
	static const int MOVE_TO_INSTANCE = 2;
	static const int GATHER_FROM_INSTANCE = 3;
	static const int MOVE_IN_SQUADRON_TO_POSITION = 4;
	static const int BUILD_INSTANCE = 5;
public:
	EntityEvent();
	~EntityEvent();

	int eventType;
	bool initialized;
	Entity* owner;

	glm::vec2 moveToPosition;

	Path path;
	Entity* entity;
	Squadron* squadron;
	Group* group;

	bool needToMove;
private:
};

#endif
