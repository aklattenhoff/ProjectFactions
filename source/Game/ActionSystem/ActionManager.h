#ifndef _ACTION_MANAGER_H
#define _ACTION_MANAGER_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"
#include "Game/World.h"

#include "Game/Graphics/Camera.h"

#include "Game/Input/Input.h"

#include "Game/ObjectManager/ObjectSelectionArray.h"
#include "Game/ObjectManager/ObjectInstance/ObjectInstanceArray.h"
#include "Game/ObjectManager/ObjectInstance/ObjectInstance.h"
#include "Game/ObjectManager/ObjectInstance/ObjectInstanceCreator.h"
#include "Game/ObjectManager/ObjectClass/ObjectClass.h"
#include "Game/ObjectManager/ObjectSelector.h"
#include "Game/ObjectManager/ObjectInstanceMap.h"

#include "Factions/FactionList.h"

#include "Game/Terrain/Terrain.h"

#include "Game/ActionManager/Action.h"
#include "Game/ActionManager/ActionManagerInterface.h"

#include "Game/ObjectManager/ObjectManagerInterface.h"

const int ACTION_MANAGER_STATE_DEFAULT = 0;
const int ACTION_MANAGER_STATE_BUILD = 1;

class EntityBase;

class ActionManager
{
public:
	ActionManager();
	~ActionManager();

	int Init();
	int ProcessInput(World* world);
	int Process(World* world, FactionList* factionList);
	int Draw(World* world, bool mouseOverGUI);

	ActionManagerInterface* GetInterface(){return actionManagerInterface;}
private:
	ActionManagerInterface* actionManagerInterface;
	ObjectInstanceCreator* objectInstanceCreator;

	int currentState;

	//Build state
	EntityBase* buildStateEntityBase;
	float buildStateRotation;
};
#endif
