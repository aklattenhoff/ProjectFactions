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
	enum entity_event_t
	{
		MOVE_TO_POSITION = 1,
		MOVE_TO_INSTANCE,
		GATHER_FROM_INSTANCE,
		MOVE_IN_SQUADRON_TO_POSITION,
		BUILD_INSTANCE,
		ATTACK_INSTANCE,
		HUNT_INSTANCE,
	};
	/*static const int MOVE_TO_POSITION = 1;
	static const int MOVE_TO_INSTANCE = 2;
	static const int GATHER_FROM_INSTANCE = 3;
	static const int MOVE_IN_SQUADRON_TO_POSITION = 4;
	static const int BUILD_INSTANCE = 5;
	static const int ATTACK_INSTANCE = 6;
	static const int ATTACK_INSTANCE = 7;*/
public:
	EntityEvent();
	~EntityEvent();

	entity_event_t eventType;
	bool initialized;
	Entity* owner;

	glm::vec2 moveToPosition;

	Path path;
	Entity* entity;
	Squadron* squadron;
	Group* group;
	int frameCount;

	//Movement info
	bool requestedPath;
	bool hasPath;

	bool needToMove;

	//Gather info
	bool foundGatherType;
	bool hasGatherArea;
	bool atGatherArea;
	bool hasSetGatherRallyPoint;
	int gatherTimer;
	float gatherRate;
private:
};

#endif
