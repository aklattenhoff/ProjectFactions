#include "EntityEventQueue.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityEventQueue::EntityEventQueue()
{
}

EntityEventQueue::~EntityEventQueue()
{
}

int EntityEventQueue::Enqueue(EntityEvent* entityEvent)
{
	entityEventQueue.push(entityEvent);

	return 0;
}

int EntityEventQueue::DequeueAndDelete()
{
	delete entityEventQueue.front();
	entityEventQueue.pop();

	return 0;
}

int EntityEventQueue::ClearAndDelete()
{
	while (entityEventQueue.size() > 0)
	{
		DequeueAndDelete();
	}

	return 0;
}

int EntityEventQueue::Size()
{
	return entityEventQueue.size();
}

EntityEvent* EntityEventQueue::GetFrontEvent()
{
	return entityEventQueue.front();
}
