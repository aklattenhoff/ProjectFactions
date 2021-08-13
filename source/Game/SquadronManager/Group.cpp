#include "Group.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/Entity.h"

Group::Group()
{
	direction = 0.0f;
	atDestination = false;
}

Group::~Group()
{
}

int Group::Init()
{
	return 0;
}

int Group::RemoveEntity(Entity* entity)
{
	for (unsigned int i = 0;i < entities.size();i ++)
	{
		if (entities[i] == entity && entities.size() > i)
			entities.erase(entities.begin()+i);
	}

	return 0;
}
