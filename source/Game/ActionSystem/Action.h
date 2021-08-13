#ifndef _ACTION_H
#define _ACTION_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

const int ACTION_TYPE_BUILD = 0;
const int ACTION_TYPE_TRAIN = 1;

class Action
{
public:
	Action();
	~Action();

	std::string getActionName(){return actionName;}
	int getActionType(){return actionType;}
	GLuint getActionIconTexture(){return iconTexture;}
	std::string getbuildObjectName(){return buildObjectName;}
	std::string gettrainObjectName(){return trainObjectName;}
private:
	friend class ActionParser;

	std::string actionName;
	int actionType;

	std::string iconFileName;
	GLuint iconTexture;

	std::string buildObjectName;
	std::string trainObjectName;
};
#endif
