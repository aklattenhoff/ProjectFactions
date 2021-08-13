#ifndef _GROUP_H
#define _GROUP_H

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Path.h"

class Entity;

class Group
{
public:
	Group();
	~Group();

	int Init();
	int RemoveEntity(Entity* entity);
public:
	std::vector<Entity*> entities;
	glm::vec2 centerpoint;
	float direction;
	Path path;
	bool atDestination;
};

#endif
