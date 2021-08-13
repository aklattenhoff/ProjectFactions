#ifndef _ENTITY_MOVEMENT_H
#define _ENTITY_MOVEMENT_H

class World;
class Entity;
class EntityEvent;

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityMovement
{
public:
	EntityMovement();
	~EntityMovement();

	int Init();
	int StartMovement(World* world, Entity* entity, EntityEvent* evnt);
	int RequestPath(glm::vec2 pointA, glm::vec2 pointB, World* world, Entity* entity, EntityEvent* evnt);
	int CalculateVectors(World* world, Entity* entity, EntityEvent* evnt);
	int CalculateMovement(World* world, Entity* entity, EntityEvent* evnt);
	int MoveEntity(World* world, Entity* entity);
	int CleanUp(World* world, Entity* entity);

	int StopMovement(World* world, Entity* entity);
private:
	bool LineCircleIntersect2D(glm::vec2 pt1, glm::vec2 magAndDir, glm::vec2 circle, float radius);
	glm::vec2 RotatePoint2D(glm::vec2 origin, glm::vec2 point, float radians);
	bool RectangleCircleIntersect2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 center, float rotation, glm::vec2 circle, float radius);
	int CalculateDestination(World* world, Entity* entity, EntityEvent* evnt);
	bool CirlceGridIntersect2D(World* world, glm::vec2 center, float radius);
private:
};

#endif
