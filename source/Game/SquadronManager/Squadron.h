#ifndef _SQUADRON_H
#define _SQUADRON_H

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class World;
class Entity;
class Group;

class Squadron
{
public:
	Squadron();
	~Squadron();

	int Init();
	int AddEntity(Entity* entity);
	int RemoveEntity(Entity* entity);
	int SetEndpoint(glm::vec2 endpoint);
	int SetEndpoint(float x, float y);
	glm::vec2 GetEndpoint();
	float GetEndpointDirection();

	int CreateSquadron(World* world);
	int UpdateSquadron(World* world);
	int DebugDrawGroups(World* world, unsigned int tex);
private:
	//Create Squadron
	int CalculateGroups(World* world);
	int CalculateActiveEntities(World* world);
	int CalculateGroupStartpoints(World* world);
	int CalculateEndpointDirection();

	//Update Squadron
	int CalculateNewActiveEntities(World* world);

	int AddActiveEntityToGroups(World* world, Entity* entity);
	int CalculateGroupMerge(World* world);
	int MergeGroups(Group* group1, Group* group2);
private:
	std::vector<Entity*> _entitiesInSquadron;
	glm::vec2 _endpoint;
	float _endpointDirection;
	std::vector<Group*> _groupVector;

	std::vector<Entity*> _activeEntities;
	int removedEntitiyNumber;
};

#endif
