#include "EntityProcessEvent.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

EntityProcessEvent::EntityProcessEvent()
{
	initialized = false;
	owner = NULL;
	entityBase = NULL;
	trainPoints = 0.0f;
	timer = 0;
}

EntityProcessEvent::~EntityProcessEvent()
{
}
