#include "Entity.h"

#include "loadGLE.h"
#include "Debug/Debug.h"

#include "Game/Entity/EntityProcessEventQueue.h"

#include "Game/Entity/EntityBase.h"
#include "Game/Entity/EntityStats.h"

Entity::Entity()
{
	_entityStats = NULL;
	_entityEventQueue = NULL;
	_entityProcessEventQueue = NULL;
	_animationIndex = 0;
	_animationSeconds = ((float)(SDL_GetTicks()) / 1000.0f);
	_needsBuild = false;
	_phageModelType = PHAGE_MODEL_TYPE_INVALID;

	_hasWayPoint = false;
	_wayPoint = glm::vec3(0.0f);

	_damageTaken = 0.0f;

	_prevPosIndex = 0;

	_isAlive = true;
	_isDying = false;
	_startedDeathAnimation = false;
}

Entity::~Entity()
{
	delete _entityStats;
	delete _entityEventQueue;
	delete _entityProcessEventQueue;
}

int Entity::Init()
{
	_entityStats = new EntityStats();
	_entityStats->SetEntityBase(_entityBase);

	_entityEventQueue = new EntityEventQueue();

	_entityProcessEventQueue = new EntityProcessEventQueue();

	_entityStatus = STATUS_IDLING;

	_isMouseOver = false;
	_isSelected = false;

	_gatherType = -1;
	_gatherOverwriteType = -1;

	_isBuilt = false;
	_currentBuildPoints = 0;

	_phageModelType = PHAGE_MODEL_TYPE_INVALID;

	return 0;
}

EntityBase* Entity::GetEntityBase()
{
	return _entityBase;
}

int Entity::GetOwningPlayer()
{
	return _owningPlayer;
}

EntityStats* Entity::GetEntityStats()
{
	return _entityStats;
}

int Entity::SetPosition(float x, float y, float z)
{
	_transformation.translation.x = x;
	_transformation.translation.y = y;
	_transformation.translation.z = z;

	//_phageEntity->SetTranslate(x, y, z);

	return 0;
}

int Entity::SetRotation(float a)
{
	_transformation.rotation.y = a;

	//_phageEntity->SetRotate(a, 0.0f, 0.0f, 1.0f);

	return 0;
}

/*int Entity::ResetToNodeVector()
{
	_toNodeVec.direction = 0.0f;
	_toNodeVec.magnitude = 0.0f;

	return 0;
}*/

/*int Entity::ResetMovementVectorMagnitude()
{
	_movementVec.magnitude = 0.0f;

	return 0;
}

int Entity::ResetMovementVector()
{
	_movementVec.direction = 0.0f;
	_movementVec.magnitude = 0.0f;

	return 0;
}*/

int Entity::SetAnimationIndex(int animIndex)
{
	if (_entityBase->GetModel()->GetAnimation(animIndex))
	{
		_animationIndex = animIndex;
		_phageEntity->SetAnimationIndex(_animationIndex);
	}
	return 0;
}

int Entity::SetGatherType(const int type)
{
	_gatherType = type;

	return 0;
}

int Entity::SetGatherOverwriteType(const int type)
{
	_gatherOverwriteType = type;

	return 0;
}

int Entity::GetGatherType()
{
	return _gatherType;
}

int Entity::GetGatherOverwriteType()
{
	return _gatherOverwriteType;
}

bool Entity::HasWayPoint()
{
	return _hasWayPoint;
}

int Entity::SetWayPoint(glm::vec3 wayPoint)
{
	_hasWayPoint = true;
	_wayPoint = wayPoint;

	return 0;
}

glm::vec3 Entity::GetWayPoint()
{
	return _wayPoint;
}

bool Entity::IsBuilt()
{
	if (!_isBuilt)
	{
		if (_needsBuild)
			_isBuilt = _currentBuildPoints >= _entityStats->BuildPoints();
		else
			_isBuilt = true;
	}
	return _isBuilt;
}

int Entity::AddCurrentBuildPoints(float bp)
{
	int testBuildPoints = (int)_currentBuildPoints;

	_currentBuildPoints += bp;

	if (testBuildPoints < (int)_currentBuildPoints)
		DebugOutput("%s build points: %f\n", _entityBase->EntityName().c_str(), _currentBuildPoints);

	return 0;
}

float Entity::GetTotalBuildPoints()
{
	return _entityStats->BuildPoints();
}

Model* Entity::GetModel()
{
	if (_needsBuild && !IsBuilt())
		return _entityBase->GetUnbuiltModel();

	return _entityBase->GetModel();
}

Phage::PhageEntity* Entity::GetPhageEntity()
{
	if (_needsBuild && !IsBuilt())
	{
		if (_phageModelType != PHAGE_MODEL_TYPE_UNBUILT)
		{
			_phageEntity->SetPhageModel(_entityBase->GetUnbuiltPhageModel());
			_phageModelType = PHAGE_MODEL_TYPE_UNBUILT;
		}
	}
	else
	{
		if (_phageModelType != PHAGE_MODEL_TYPE_BUILT)
		{
			_phageEntity->SetPhageModel(_entityBase->GetPhageModel());
			_phageModelType = PHAGE_MODEL_TYPE_BUILT;
		}
	}

	_phageEntity->SetIsSelected(_isSelected);

	return _phageEntity;
}

int Entity::SetProjectileCreated(int i, bool f)
{
	if (i < 32)
		_projectileCreated[i] = f;

	return 0;
}

bool Entity::GetProjectileCreated(int i)
{
	if (i < 32)
		return _projectileCreated[i];

	return false;
}

int Entity::ClearAllProjectileCreated()
{
	memset(_projectileCreated, 0, sizeof(_projectileCreated));

	return 0;
}

int Entity::AddPreMitigatedDamage(float damage, int type)
{
	float armorValue = 0.0f;

	switch (type)
	{
	case 0:
		armorValue = _entityBase->GetEntityXML()->hackArmor;
		break;
	case 1:
		armorValue = _entityBase->GetEntityXML()->pierceArmor;
		break;
	case 2:
		armorValue = _entityBase->GetEntityXML()->magicArmor;
		break;
	}

	if (armorValue > 0.95f)
		armorValue = 0.95f;

	float postMitigatedDamage = damage - (damage * armorValue);

	_damageTaken += postMitigatedDamage;

	DebugOutput("Took %1.2f damage: %1.2f\n", postMitigatedDamage, _damageTaken);

	if (_damageTaken >= 10.0f)
	{
		_isAlive = false;
		_isDying = true;
	}

	return 0;
}
