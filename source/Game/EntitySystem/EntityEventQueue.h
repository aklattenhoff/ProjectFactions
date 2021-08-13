#ifndef _ENTITY_EVENT_QUEUE_H
#define _ENTITY_EVENT_QUEUE_H

#include <queue>

#include "Game/EntitySystem/EntityEvent.h"

class EntityEventQueue
{
public:
	EntityEventQueue();
	~EntityEventQueue();

	int Enqueue(EntityEvent* entityEvent);
	int DequeueAndDelete();
	int ClearAndDelete();
	int Size();
	EntityEvent* GetFrontEvent();
private:
	std::queue<EntityEvent*> entityEventQueue;
};
#endif
