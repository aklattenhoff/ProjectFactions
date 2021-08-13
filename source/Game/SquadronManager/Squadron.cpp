#include "Squadron.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/World.h"

#include "Game/Entity/Entity.h"
#include "Game/Entity/EntityEvent.h"
#include "Game/Entity/EntityEventQueue.h"

#include "Game/SquadronManager/Group.h"

//#include "Game/Terrain/Terrain.h"

Squadron::Squadron()
{
	//This way we can check if the direction has been set prior to calculating
	_endpointDirection = -1.0f;

	removedEntitiyNumber = 0;
}

Squadron::~Squadron()
{
	for (unsigned int i = 0;i < _groupVector.size();i ++)
		delete _groupVector[i];
}

int Squadron::Init()
{
	return 0;
}

int Squadron::AddEntity(Entity* entity)
{
	_entitiesInSquadron.push_back(entity);

	return 0;
}

int Squadron::RemoveEntity(Entity* entity)
{
	DebugOutput("Removing entity from squad\n");
	for (unsigned int i = 0;i < _entitiesInSquadron.size();i ++)
	{
		if (_entitiesInSquadron[i] == entity)
		{
			_entitiesInSquadron.erase(_entitiesInSquadron.begin()+i);
			removedEntitiyNumber ++;
			break;
		}
	}

	DebugOutput("Entities remaining:%i\n", _entitiesInSquadron.size());

	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		_groupVector[i]->RemoveEntity(entity);
	}

	return 0;
}

int Squadron::SetEndpoint(glm::vec2 endpoint)
{
	_endpoint = endpoint;

	return 0;
}

int Squadron::SetEndpoint(float x, float y)
{
	_endpoint.x = x;
	_endpoint.y = y;

	return 0;
}

glm::vec2 Squadron::GetEndpoint()
{
	return _endpoint;
}

float Squadron::GetEndpointDirection()
{
	return _endpointDirection;
}

int Squadron::CreateSquadron(World* world)
{
	CalculateActiveEntities(world);
	CalculateGroups(world);
	CalculateGroupStartpoints(world);

	if (_groupVector.size() > 0)
		CalculateEndpointDirection();

	return 0;
}

int Squadron::UpdateSquadron(World* world)
{
	//Return if squad is empty
	if (_entitiesInSquadron.size() == 0)
	{
		return 1;
	}

	CalculateGroupMerge(world);

	//Remove empty groups if there are more than one
	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		if (_groupVector.size() > 1 && _groupVector[i]->entities.size() == 0)
		{
			delete _groupVector[i];
			_groupVector.erase(_groupVector.begin() + i);
			i--;
		}
	}

	CalculateNewActiveEntities(world);

	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		//Find a path if there is not one for the group
		if (_groupVector[i]->path.Size() == 0 && !_groupVector[i]->atDestination)
		{
			DebugOutput("Finding a path for the group\n");
			//world->GetTerrain()->getTerrainMap()->FindPath(_groupVector[i]->centerpoint, _endpoint, &_groupVector[i]->path);
			world->GetPathFinder()->FindPath(_groupVector[i]->centerpoint, _endpoint, &_groupVector[i]->path);
			if (_groupVector[i]->path.Size() > 1)
				_groupVector[i]->path.EraseFront();
		}
		else if (_groupVector[i]->atDestination)
		{
			continue;
		}

		//Calculate move vector for group
		EuclideanVector2D toNodeVec;
		toNodeVec.magnitude = 0.01f;
		toNodeVec.direction = glm::degrees(atan2(_groupVector[i]->path.Front().y - _groupVector[i]->centerpoint.y, _groupVector[i]->path.Front().x - _groupVector[i]->centerpoint.x));
		_groupVector[i]->direction = -toNodeVec.direction;

		//Move the group in the direction of the move vector
		_groupVector[i]->centerpoint.x += toNodeVec.magnitude*cos(glm::radians(toNodeVec.direction));
		_groupVector[i]->centerpoint.y += toNodeVec.magnitude*sin(glm::radians(toNodeVec.direction));

		//Check if we are at the node in the path
		if (sqrt(pow(_groupVector[i]->path.Front().x - _groupVector[i]->centerpoint.x, 2) + pow(_groupVector[i]->path.Front().y - _groupVector[i]->centerpoint.y, 2) < toNodeVec.magnitude))
		{
			_groupVector[i]->centerpoint.x = _groupVector[i]->path.Front().x;
			_groupVector[i]->centerpoint.y = _groupVector[i]->path.Front().y;
			_groupVector[i]->path.EraseFront();

			//We are done moving in the group
			//Dispatch if there is only one group
			if (_groupVector[i]->path.Size() <= 0)
			{
				_groupVector[i]->atDestination = true;
				if (_groupVector.size() == 1)
				{
					while (_groupVector[i]->entities.size() > 0)
					{
						_groupVector[i]->entities[0]->SetEntityStatus(Entity::STATUS_IDLING);
						_groupVector[i]->entities[0]->GetEventQueue()->DequeueAndDelete();
						//RemoveEntity(_groupVector[i]->entities[0]);
					}
					continue;
				}
			}
		}

		//Update the entities in the group to move to their positions
		float startOffset = -((float)_groupVector[i]->entities.size() - 1.0f) / 2.0f;
		for (unsigned int j = 0;j < _groupVector[i]->entities.size();j ++)
		{
			glm::vec2 position = _groupVector[i]->centerpoint;

			//Translate for the x direction
			position.x += (startOffset+(float)j)*cos(glm::radians(toNodeVec.direction+90.0f));
			position.y += (startOffset+(float)j)*sin(glm::radians(toNodeVec.direction+90.0f));

			if (_groupVector[i]->entities[j]->GetEventQueue()->GetFrontEvent()->path.Size() == 0)
				_groupVector[i]->entities[j]->GetEventQueue()->GetFrontEvent()->path.PushBack(Path::Node());
			_groupVector[i]->entities[j]->GetEventQueue()->GetFrontEvent()->path.SetAt(0, position);
		}
	}

	return 0;
}

int Squadron::DebugDrawGroups(World* world, unsigned int tex)
{
	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		//world->GetTerrain()->getTerrainOverlay()->AddToOverlay(tex, _groupVector[i]->centerpoint.x, _groupVector[i]->centerpoint.y, (float)_groupVector[i]->entities.size(), (float)_groupVector[i]->entities.size(), _groupVector[i]->direction);
	}

	return 0;
}

int Squadron::CalculateGroups(World* world)
{
	_groupVector.clear();

	//Calculate the entities that have this squad as their front event
	//CalculateActiveEntities(world);

	//Clear all of the cached groups in the events
	for (unsigned int i = 0;i < _activeEntities.size();i ++)
	{
		_activeEntities[i]->GetEventQueue()->GetFrontEvent()->group = NULL;
	}

	//Iterate through the active entities and group them
	for (unsigned int i = 0;i < _activeEntities.size();i ++)
	{
		EntityEvent* entityEvent = _activeEntities[i]->GetEventQueue()->GetFrontEvent();
		if (entityEvent->group == NULL)
		{
			//NOTE:This could get messy because the pointer could outlive the instance.
			//But the assumption is that the event is deleted at the same time the Squadron is deleted
			//Otherwise, the event's group pointer should be cleared when the group vector is cleared
			_groupVector.push_back(new Group());
			entityEvent->group = _groupVector.back();

			entityEvent->group->entities.push_back(_activeEntities[i]);
			DebugOutput("Added new group number:%i\n", entityEvent->group);
		}
		for (unsigned int j = i + 1;j < _activeEntities.size();j ++)
		{
			if (_activeEntities[j]->GetEventQueue()->GetFrontEvent()->group != NULL)
				continue;

			glm::vec2 t1 = glm::vec2(_activeEntities[i]->GetTranslationVector().x, _activeEntities[i]->GetTranslationVector().z);
			glm::vec2 t2 = glm::vec2(_activeEntities[j]->GetTranslationVector().x, _activeEntities[j]->GetTranslationVector().z);
			if (glm::distance(t1, t2) < 10.0f/*threshold value*/)
			{
				_activeEntities[j]->GetEventQueue()->GetFrontEvent()->group = entityEvent->group;
				entityEvent->group->entities.push_back(_activeEntities[j]);
				DebugOutput("Added entity to group number:%i\n", entityEvent->group);
			}
		}
	}

	//Set the centerpoint of each group
	//CalculateGroupStartpoints(world);

	//if (_groupVector.size() > 0)
		//CalculateEndpointDirection();

	return 0;
}

int Squadron::CalculateActiveEntities(World* world)
{
	_activeEntities.clear();

	//Calculate the entities that have this squad as their front event
	for (unsigned int i = 0;i < _entitiesInSquadron.size();i ++)
	{
		if (_entitiesInSquadron[i]->GetEventQueue()->GetFrontEvent()->squadron == this)
			_activeEntities.push_back(_entitiesInSquadron[i]);
	}

	return 0;
}

int Squadron::CalculateGroupStartpoints(World* world)
{
	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		//Simply choose the entity who is closes to the goal
		float closestDistance = -1.0f;
		int closestEntityIndex = -1;
		for (unsigned int j = 0;j < _groupVector[i]->entities.size();j ++)
		{
			glm::vec2 entityPos = glm::vec2(_groupVector[i]->entities[j]->GetTranslationVector().x, _groupVector[i]->entities[j]->GetTranslationVector().z);
			float distance = glm::distance(entityPos, _endpoint);
			if (j == 0)
			{
				closestDistance = distance;
				closestEntityIndex = 0;
			}
			else
			{
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestEntityIndex = j;
				}
			}
		}

		//Set the group startpoint to the closest-to-goal point in the group
		//NOTE:The reason this is chosen is because we can guarantee that this point is free
		//and the path should be shortest for most cases.
		_groupVector[i]->centerpoint = glm::vec2(	_groupVector[i]->entities[closestEntityIndex]->GetTranslationVector().x,
													_groupVector[i]->entities[closestEntityIndex]->GetTranslationVector().z);
	}

	return 0;
}

int Squadron::CalculateEndpointDirection()
{
	//Don't calculate this if the endpoint direction is already set
	if (_endpointDirection >= 0.0f)
		return 1;

	//Find the largest group to determine the influence of the direction
	int largestGroupIndex = 0;
	for (unsigned int i = 1;i < _groupVector.size();i ++)
	{
		if (_groupVector[i]->entities.size() > _groupVector[largestGroupIndex]->entities.size())
			largestGroupIndex = i;
	}

	//Calculate the direction from the endpoint to the largest group's startpoint
	glm::vec2 startpoint = _groupVector[largestGroupIndex]->centerpoint;
	float opp = startpoint.y - _endpoint.y;
	bool oppNeg = opp < 0.0f;
	if (oppNeg) opp *= -1.0f;
	float adj = startpoint.x - _endpoint.x;
	bool adjNeg = adj < 0.0f;
	if (adjNeg) adj *= -1.0f;
	if (adj == 0.0f)
	{
		if (oppNeg)
			_endpointDirection = 270.0f;
		else
			_endpointDirection = 90.0f;
	}
	else
	{
		float angle  = glm::degrees(glm::atan(opp/adj));
		if (oppNeg)
		{
			if (adjNeg)	//QUAD 1
				angle = 360.0f - angle;
			else		//QUAD 2
				angle = 180.0f + angle;
		}
		else
		{
			if (adjNeg)	//QUAD 3
				angle = angle;
			else		//QUAD 4
				angle = 180.0f - angle;
		}
		_endpointDirection = angle;
	}

	return 0;
}

int Squadron::CalculateNewActiveEntities(World* world)
{
	//Determine if any new entities have become active
	int previousSize = _activeEntities.size();
	int expectedSize = previousSize - removedEntitiyNumber;
	CalculateActiveEntities(world);
	int newSize = _activeEntities.size();

	//If there are more people than the old size minus the number we know left the squad
	if (newSize > expectedSize)
	{
		printf("FOUND NEW ACTIVE ENTITY\n");
		//Add the new active entities to groups
		int newEntityNumber = newSize - expectedSize;

		//NOTE: This looks odd, but the newEntityNumber has to be at least one at this point.
		//So, we will be guarenteed to start this loop at least at the last element
		for (int i = newSize - newEntityNumber;i < newSize;i ++)
		{
			AddActiveEntityToGroups(world, _activeEntities[i]);
		}
	}

	//Reset removedEntityNumber
	removedEntitiyNumber = 0;

	return 0;
}

int Squadron::AddActiveEntityToGroups(World* world, Entity* entity)
{
	//Determine if the entity can be merged into an existing group
	glm::vec2 entPos = glm::vec2(entity->GetTranslationVector().x, entity->GetTranslationVector().z);
	int shortestGroupIndex = -1;
	float shortestGroupDistance = -1.0f;
	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		if (i == 0)
		{
			shortestGroupIndex = 0;
			shortestGroupDistance = glm::distance(entPos, _groupVector[i]->centerpoint);
		}
		else
		{
			float dist = glm::distance(entPos, _groupVector[i]->centerpoint);
			if (dist < shortestGroupDistance)
			{
				shortestGroupIndex = i;
				shortestGroupDistance = dist;
			}
		}
	}

	if (shortestGroupDistance >= 0.0f && shortestGroupDistance < 10.0f/*threshold value*/)
	{
		entity->GetEventQueue()->GetFrontEvent()->group = _groupVector[shortestGroupIndex];
		_groupVector[shortestGroupIndex]->entities.push_back(entity);
		DebugOutput("Added entity to group number:%i\n", _groupVector[shortestGroupIndex]);
	}
	else
	{
		//Create new group
		_groupVector.push_back(new Group());
		entity->GetEventQueue()->GetFrontEvent()->group = _groupVector.back();
		_groupVector.back()->entities.push_back(entity);
		_groupVector.back()->centerpoint = entPos;
		DebugOutput("Added new group number:%i\n", _groupVector.back());
	}

	return 0;
}

int Squadron::CalculateGroupMerge(World* world)
{
	for (unsigned int i = 0;i < _groupVector.size();i ++)
	{
		float distanceToGoal = glm::distance(_groupVector[i]->centerpoint, _endpoint);
		for (unsigned int j = i + 1;j < _groupVector.size();j ++)
		{
			float distance = glm::distance(_groupVector[i]->centerpoint, _groupVector[j]->centerpoint);
			if (distance < 10.0f/*threshold value*/)
			{
				if (glm::distance(_groupVector[j]->centerpoint, _endpoint) < distanceToGoal)
				{
					MergeGroups(_groupVector[j], _groupVector[i]);
				}
				else
				{
					MergeGroups(_groupVector[i], _groupVector[j]);
				}

				//Only merge one group per frame for now
				return 1;
			}
		}
	}

	return 0;
}

int Squadron::MergeGroups(Group* group1, Group* group2)
{
	while (group2->entities.size() > 0)
	{
		group1->entities.push_back(group2->entities[0]);
		group2->RemoveEntity(group2->entities[0]);
	}

	return 0;
}
