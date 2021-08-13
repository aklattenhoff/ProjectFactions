#ifndef _ACTION_MANAGER_H
#define _ACTION_MANAGER_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "Game/World.h"

//#include "Graphics/Camera.h"

#include "Game/Input/Input.h"

//#include "Game/Terrain/Terrain.h"

#include "Game/ActionManager/Action.h"
#include "Game/ActionManager/ActionManagerInterface.h"

#include "Phage/Phage.h"

const int ACTION_MANAGER_STATE_DEFAULT = 0;
const int ACTION_MANAGER_STATE_BUILD = 1;
const int ACTION_MANAGER_STATE_BUILD_WALL = 2;
const int ACTION_MANAGER_STATE_BUILD_WALL_PHASE_2 = 3;

class EntityBase;

class ActionManager
{
private:
	struct WallSegment
	{
		glm::vec3 pos;
		std::vector<int> conns;
	};
public:
	ActionManager();
	~ActionManager();

	int Init();
	int ProcessInput(World* world);
	int Process(World* world, FactionList* factionList);
	int Draw(World* world, bool mouseOverGUI);

	ActionManagerInterface* GetInterface(){return actionManagerInterface;}
private:
	int CreateInstanceAndAddToQueue(World* world, EntityBase* entBase, glm::vec2 pos, float rotation, bool clearQueue);
private:
	ActionManagerInterface* actionManagerInterface;

	int currentState;

	//Build state
	EntityBase* buildStateEntityBase;
	EntityBase* buildStateEntityBaseWallConnector;
	EntityBase* buildStateEntityBaseWall2x1;
	EntityBase* buildStateEntityBaseWall4x1;
	EntityBase* buildStateEntityBaseWall6x1;
	Phage::PhageEntity* _buildStatePhageWallConnectorEntity;
	Phage::PhageModel* _buildStatePhageWallConnectorModel;
	float buildStateRotation;
	bool startFromExistingConnector;
	bool FinishAtExistingConnector;
	std::vector<WallSegment> _wallSegmentVector;
	int _currentWallSegment;
	int _finishWallTimer;
	int _hoverTower;

	Phage::PhageEntity* _buildStatePhageEntity;
	Phage::PhageModel* _buildStatePhageModel;

	glm::vec3 wallAnchorPoint;
};
#endif
