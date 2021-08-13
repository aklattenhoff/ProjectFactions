#ifndef _ENTITY_SELECTION_HANDLER_H
#define _ENTITY_SELECTION_HANDLER_H

#include <vector>

#include "Game/World.h"
#include "Game/Entity/Entity.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntitySelectionHandler
{
public:
	EntitySelectionHandler();
	~EntitySelectionHandler();

	//Initializes the entity selection
	int Init();

	int SetOrthoSquare(World* world, glm::vec4* points);
	int CalcMouseOver(World* world);
	int SelectMouseOver(World* world);
	int ClearMouseOver(World* world);
private:
	int CalcRayIntersect(World* world);
	int CalcOrthoSquareIntersect(World* world);
private:
	bool _entitySelectionFlag;
	glm::vec4 _entitySelectionPoints;

	//std::vector<Entity*> _entityMouseOverVector;
};

#endif
