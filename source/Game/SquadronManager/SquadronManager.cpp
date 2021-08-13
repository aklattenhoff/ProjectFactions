#include "SquadronManager.h"

#include "Game/SquadronManager/Squadron.h"

#include "Texturer/TextureLoader.h"
//#include "Game/Terrain/Terrain.h"

SquadronManager::SquadronManager()
{
}

SquadronManager::~SquadronManager()
{
}

int SquadronManager::Init()
{
	_debugTexture = TextureLoader_LoadFromFile("textures/Squadron.png");
	_debugGroupTexture = TextureLoader_LoadFromFile("textures/Group.png");

	return 0;
}

Squadron* SquadronManager::CreateSquadron()
{
	_squadronVector.push_back(new Squadron());
	_squadronVector.back()->Init();

	return _squadronVector.back();
}

int SquadronManager::Draw(World* world)
{
	//Check for squadrons without people
	for (unsigned int i = 0;i < _squadronVector.size();i ++)
	{
		if (_squadronVector[i]->UpdateSquadron(world) == 1)
		{
			delete _squadronVector[i];
			_squadronVector.erase(_squadronVector.begin()+i);
		}
	}

	for (unsigned int i = 0;i < _squadronVector.size();i ++)
	{
		//_squadronVector[i]->DebugDrawGroups(world, _debugGroupTexture);
		//world->GetTerrain()->getTerrainOverlay()->AddToOverlay(_debugTexture, _squadronVector[i]->GetEndpoint().x, _squadronVector[i]->GetEndpoint().y, 5.0f, 5.0f, _squadronVector[i]->GetEndpointDirection());
	}

	return 0;
}
