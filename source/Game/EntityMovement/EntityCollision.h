#ifndef _ENTITY_COLLISION_H
#define _ENTITY_COLLISION_H

class World;
class Entity;
class EntityEvent;

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityCollision
{
public:
	EntityCollision();
	~EntityCollision();

	int Init();
	int StartCollisionCheck(World* world, Entity* entity);
	int CalculateEntityCollisions(World* world, Entity* entity);
	int CalculateBuildingCollisions(World* world, Entity* entity);
	int CalculateMapCollisions(World* world, Entity* entity);
	int HandleCollisions(World* world, Entity* entity);
private:
	glm::vec2 RotatePoint2D(glm::vec2 origin, glm::vec2 point, float radians);
	bool RectangleCircleIntersect2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 center, float rotation, glm::vec2 circle, float radius, glm::vec2* resolvePosition);
	bool CirlceGridIntersect2D(World* world, glm::vec2 center, float radius, glm::vec2* gridPosOut);
private:
};

#endif
