#ifndef _ENTITY_RENDERER_H
#define _ENTITY_RENDERER_H

#include "Game/Graphics/Camera.h"
#include "Game/EntitySystem/EntityList.h"
#include "Game/EntitySystem/Entity.h"
#include "Game/ObjectManager/Indicator/HealthIndicator.h"
#include "Game/World.h"

class EntityRenderer
{
public:
	EntityRenderer();
	~EntityRenderer();

	int Init();

	int RenderEntities(World* world, Camera* camera, EntityList* entityList);
private:
	int RenderHealthIndicator(Camera* camera, Entity* entity);
private:
	HealthIndicator* _healthIndicator;
	bool glow;
	bool bump;
	bool spec;
};

#endif
