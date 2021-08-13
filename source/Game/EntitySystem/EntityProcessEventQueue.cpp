#include "EntityProcessEventQueue.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/EntitySystem/EntityProcessEvent.h"

EntityProcessEventQueue::EntityProcessEventQueue()
{
}

EntityProcessEventQueue::~EntityProcessEventQueue()
{
}

int EntityProcessEventQueue::Enqueue(EntityProcessEvent* entityEvent)
{
	entityEventQueue.push(entityEvent);

	return 0;
}

int EntityProcessEventQueue::DequeueAndDelete()
{
	delete entityEventQueue.front();
	entityEventQueue.pop();

	return 0;
}

int EntityProcessEventQueue::ClearAndDelete()
{
	while (entityEventQueue.size() > 0)
	{
		DequeueAndDelete();
	}

	return 0;
}

int EntityProcessEventQueue::Size()
{
	return entityEventQueue.size();
}

EntityProcessEvent* EntityProcessEventQueue::GetFrontEvent()
{
	return entityEventQueue.front();
}
