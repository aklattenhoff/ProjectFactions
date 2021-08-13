#ifndef _ACTION_PROGRESS_QUEUE_H
#define _ACTION_PROGRESS_QUEUE_H

#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class EntityProcessEvent;

class ActionProgressQueue
{
public:
	ActionProgressQueue();
	~ActionProgressQueue();

	int Init();
	int AddActionProgress(EntityProcessEvent* entProcEvnt, unsigned int textureID);
	int Size();
	int GetActionProgress(int index, EntityProcessEvent** entProcEvnt, unsigned int* textureID, int* count);
	int RemoveActionProgress(EntityProcessEvent* entProcEvnt);
private:
	struct ActionProgress
	{
		std::vector<EntityProcessEvent*> entityProcessEventVector;
		unsigned int textureID;
	};

	std::vector<ActionProgress> _actionProgressVector;
};
#endif
