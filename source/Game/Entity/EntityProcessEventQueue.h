#ifndef _ENTITY_PROCESS_EVENT_QUEUE_H
#define _ENTITY_PROCESS_EVENT_QUEUE_H

#include <queue>

class EntityProcessEvent;

class EntityProcessEventQueue
{
public:
	EntityProcessEventQueue();
	~EntityProcessEventQueue();

	int Enqueue(EntityProcessEvent* entityEvent);
	int DequeueAndDelete();
	int ClearAndDelete();
	int Size();
	EntityProcessEvent* GetFrontEvent();
private:
	std::queue<EntityProcessEvent*> entityEventQueue;
};
#endif
