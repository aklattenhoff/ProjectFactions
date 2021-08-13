#ifndef _ENTITY_H
#define _ENTITY_H

#include <SDL/SDL.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Transformation.h"
#include "Game/Math/EuclideanVector2D.h"

#include "Game/Entity/EntityEventQueue.h"

class EntityProcessEventQueue;

class EntityBase;
class EntityStats;
class Model;

//#include "Game/Terrain/TerrainMap.h"
#include "Game/PathFinder/PathFinder.h"

#include "Phage/Phage.h"

class Entity
{
public:
	const static int STATUS_IDLING = 0;
	const static int STATUS_WALKING = 1;
	const static int STATUS_GATHERING = 2;
	const static int STATUS_ATTACKING = 3;
	const static int STATUS_BUILDING = 4;

public:
	Entity();
	~Entity();

	int Init();

	EntityBase* GetEntityBase();
	int GetOwningPlayer();
	EntityStats* GetEntityStats();

	Transformation GetTransformation() {return _transformation;}
	glm::vec3 GetTranslationVector() {return _transformation.translation;}

	//int SetPosX(float x) {_transformation.translation.x = x; _phageEntity->SetTranslate(_transformation.translation.x, _transformation.translation.y, _transformation.translation.z); return 0;}
	//int SetPosY(float y) {_transformation.translation.y = y; _phageEntity->SetTranslate(_transformation.translation.x, _transformation.translation.y, _transformation.translation.z); return 0;}
	//int SetPosZ(float z) {_transformation.translation.z = z; _phageEntity->SetTranslate(_transformation.translation.x, _transformation.translation.y, _transformation.translation.z); return 0;}
	int SetPosition(float x, float y, float z);

	float PosX() {return _transformation.translation.x;}
	float PosY() {return _transformation.translation.y;}
	float PosZ() {return _transformation.translation.z;}

	glm::vec2 Pos2D() {return glm::vec2(_transformation.translation.x, _transformation.translation.z);}

	//int SetRotX(float x) {_transformation.rotation.x = x; return 0;}
	//int SetRotY(float y) {_transformation.rotation.y = y; return 0;}
	//int SetRotZ(float z) {_transformation.rotation.z = z; return 0;}
	int SetRotation(float a);

	float RotX() {return _transformation.rotation.x;}
	float RotY() {return _transformation.rotation.y;}
	float RotZ() {return _transformation.rotation.z;}

	//int ResetToNodeVector();
	//int SetToNodeVector(EuclideanVector2D moveVec) {_toNodeVec = moveVec; return 0;}
	//EuclideanVector2D GetToNodeVector() {return _toNodeVec;}

	//int ResetMovementVectorMagnitude();
	//int ResetMovementVector();
	//int SetMovementVector(EuclideanVector2D moveVec) {_movementVec = moveVec; return 0;}
	//EuclideanVector2D GetMovementVector() {return _movementVec;}
	//int TurnMovementVector(bool positive, bool collision) {if (positive) _movementVec.direction += 5.0f; else _movementVec.direction -= 5.0f; return 0;}

	//Accessors
	EntityEventQueue* GetEventQueue() {return _entityEventQueue;}
	EntityProcessEventQueue* GetProcessEventQueue() {return _entityProcessEventQueue;}

	int SetEntityStatus(int status) {_entityStatus = status; return 0;}
	int GetEntityStatus() {return _entityStatus;}
	int SetCachedEntityStatus(int status) {_cachedEntityStatus = status; return 0;}
	int GetCachedEntityStatus() {return _cachedEntityStatus;}
	bool HasEntityStatusChanged() {return _cachedEntityStatus != _entityStatus;}

	int SetAnimationIndex(int animIndex);
	int GetAnimationIndex() {return _animationIndex;}
	int ResetAnimationSecond() { _animationSeconds = ((float)SDL_GetTicks() / 1000.0f); _animationCycle = 0; return 0; }
	float GetAnimationSeconds() { return ((float)SDL_GetTicks() / 1000.0f) - _animationSeconds; }
	int SetAnimationSecond(float secs) { _animationSeconds = secs; return 0; }
	int SetAnimationProgress(float animProgress) {_animationProgress = animProgress; return 0;}
	float GetAnimationProgress() {return _animationProgress;}

	int SetAnimationCycle(int ac) { _animationCycle = ac; return 0; }
	int GetAnimationCycle() { return _animationCycle; }

	int SetIsMouseOver(bool flag) {_isMouseOver = flag; return 0;}
	bool IsMouseOver() {return _isMouseOver;}
	int SetIsSelected(bool flag) {_isSelected = flag; return 0;}
	bool IsSelected() {return _isSelected;}

	int SetGatherType(const int type);
	int SetGatherOverwriteType(const int type);
	int GetGatherType();
	int GetGatherOverwriteType();

	bool HasWayPoint();
	int SetWayPoint(glm::vec3 wayPoint);
	glm::vec3 GetWayPoint();

	bool IsBuilt();
	int SetCurrentBuildPoints(float bp) {_currentBuildPoints = bp; return 0;}
	int AddCurrentBuildPoints(float bp);
	float GetCurrentBuildPoints() {return _currentBuildPoints;}

	float GetTotalBuildPoints();

	int SetNeedsToCreateProjectile(bool flag) {_needsToCreateProjectile = flag;return 0;}
	bool GetNeedsToCreateProjectile() {return _needsToCreateProjectile;}

	int SetProjectilesCreatedThisCycle(int pc) { _projectilesCreatedThisCycle = pc; return 0; }
	int AddProjectilesCreatedThisCycle() { _projectilesCreatedThisCycle++; }
	int GetProjectilesCreatedThisCycle() { return _projectilesCreatedThisCycle; }
	int SetProjectileCreated(int i, bool f);
	bool GetProjectileCreated(int i);
	int ClearAllProjectileCreated();

	int SetStartedAttack(bool f) { _startedAttack = f; return 0; }
	bool GetStartedAttack() { return _startedAttack; }

	int SetAttackTimer(unsigned int t) { _attackTimer = t; return 0; }
	unsigned int GetAttackTimer() { return _attackTimer; }

	int AddPreMitigatedDamage(float damage, int type);
	float GetDamageTaken() { return _damageTaken; }
	bool IsAlive() { return _isAlive; }
	bool IsDying() { return _isDying; }
	bool SetIsDying(bool flag) { _isDying = flag; return 0; }
	bool StartedDeathAnimation() { return _startedDeathAnimation; }
	int SetStartedDeathAnimation(bool flag) { _startedDeathAnimation = flag; return 0; }

	Model* GetModel();
	Phage::PhageEntity* GetPhageEntity();

	//TerrainMapCellsVector* GetTerrainMapCellsVectorPointer() {return &_terrainMapCellsVector;}
	PathFinder::ObstacleSubsetGroup* GetObstacleSubsetGroupPointer() { return &_obstacleSubsetGroup; }

private:
	const static int PHAGE_MODEL_TYPE_INVALID = -1;
	const static int PHAGE_MODEL_TYPE_UNBUILT = 0;
	const static int PHAGE_MODEL_TYPE_BUILT = 1;
private:
	friend class EntityBase;

	//These are for ease of interdependent systems, but should probably make accessors/mutators instead
	friend class EntityMovement;
	friend class EntityCollision;

	//Pointer to the instance creator
	EntityBase* _entityBase;

	int _owningPlayer;

	EntityStats* _entityStats;

	//Transformation of the entity
	Transformation _transformation;

	//All movement related variables
	glm::vec2 _desiredDestination;
	glm::vec2 _currentDestination;
	bool _destinationFlag;
	int _destinationCheck;
	float _destinationCheckDirection;
	float _destinationCheckDirectionStep;
	float _destinationCheckDistance;
	//EuclideanVector2D _toNodeVec;
	//EuclideanVector2D _movementVec;
	glm::vec2 _moveToNodeVec;
	glm::vec2 _moveCurrentVec;
	float _awareDistance;
	glm::vec2 _steerCurrentVec;
	glm::vec2 _steerLeftPoint;
	glm::vec2 _steerRightPoint;
	bool _hasTargetNode;
	Path::Node _targetNode;
	float _distanceToTargetNode;
	int _targetNodeCheckValue;
	bool _giveUpAndPushThrough;
	int _oscillationPrevent;
	Entity* _currentObstruction;
	Entity* _previousObstruction;
	int _amountToTurn;
	int _moveCheckCount;
	int _stuckCounter;
	int _stopAndWaitCounter;
	bool _watchingObstacle;
	float _obstacleDirection;
	
	int _prevPosIndex;
	glm::vec2 _previousPositionList[5];

	unsigned int _stuckTime;
	glm::vec2 _stuckPos;

	//All collision related variables
	glm::vec3 _collisionVector;
	int _collisionNumber;
	bool _collisionWithMap;
	glm::vec3 _collisionMapVector;
	int _collisionMapNumber;

	//Event queue
	EntityEventQueue* _entityEventQueue;

	//Process event queue
	EntityProcessEventQueue * _entityProcessEventQueue;

	int _entityStatus;
	int _cachedEntityStatus;

	int _animationIndex;
	float _animationSeconds;
	float _animationProgress;
	int _animationCycle;

	bool _isMouseOver;
	bool _isSelected;

	float _radius;

	bool _needsBuild;
	bool _isBuilt;
	float _currentBuildPoints;

	bool _needsToCreateProjectile;
	int _projectilesCreatedThisCycle;
	bool _projectileCreated[32];
	bool _startedAttack;
	unsigned int _attackTimer;

	//TerrainMapCellsVector _terrainMapCellsVector;
	PathFinder::ObstacleSubsetGroup _obstacleSubsetGroup;

	Phage::PhageEntity* _phageEntity;
	int _phageModelType;

	int _gatherType;
	int _gatherOverwriteType;

	bool _hasWayPoint;
	glm::vec3 _wayPoint;

	float _damageTaken;
	bool _isAlive;
	bool _isDying;
	bool _startedDeathAnimation;
};

#endif
