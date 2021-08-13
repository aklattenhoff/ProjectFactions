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
const int ACTION_TYPE_BUILD_WALL = 2;
const int ACTION_TYPE_UPGRADE = 3;

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
	std::string getbuildWallConnectorObjectName(){return buildWallConnectorObjectName;}
	std::string getbuildWall2x1ObjectName(){return buildWall2x1ObjectName;}
	std::string getbuildWall4x1ObjectName(){return buildWall4x1ObjectName;}
	std::string getbuildWall6x1ObjectName(){return buildWall6x1ObjectName;}
	std::string getupgradeObjectName(){return upgradeObjectName;}
	float getupgradeTime(){return upgradeTime;}
private:
	friend class ActionParser;
	friend class ActionListCreator;

	std::string actionName;
	int actionType;

	std::string iconFileName;
	GLuint iconTexture;

	std::string buildObjectName;
	std::string trainObjectName;

	std::string buildWallConnectorObjectName;
	std::string buildWall2x1ObjectName;
	std::string buildWall4x1ObjectName;
	std::string buildWall6x1ObjectName;

	std::string upgradeObjectName;
	float upgradeTime;
};
#endif
