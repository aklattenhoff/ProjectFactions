#ifndef _ENTITY_ANIMATION_HANDLER_H
#define _ENTITY_ANIMATION_HANDLER_H

class Entity;

class EntityAnimationHandler
{
public:
	EntityAnimationHandler();
	~EntityAnimationHandler();

	int Init();
	int ProcessAnimation(Entity* entity);
private:
};

#endif
