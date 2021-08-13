#ifndef _ENTITY_H
#define _ENTITY_H

#include <SDL/SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Game/Math/Transformation.h"
#include "Game/Math/EuclideanVector2D.h"

#include "Game/EntitySystem/EntityEventQueue.h"

class EntityProcessEventQueue;

class EntityBase;
class EntityStats;

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
	EntityStats* GetEntityStats();

	Transformation GetTransformation() {return _transformation;}
	glm::vec3 GetTranslationVector() {return _transformation.translation;}

	int SetPosX(float x) {_transformation.translation.x = x; return 0;}
	int SetPosY(float y) {_transformation.translation.y = y; return 0;}
	int SetPosZ(float z) {_transformation.translation.z = z; return 0;}

	float PosX() {return _transformation.translation.x;}
	float PosY() {return _transformation.translation.y;}
	float PosZ() {return _transformation.translation.z;}

	glm::vec2 Pos2D() {return glm::vec2(_transformation.translation.x, _transformation.translation.z);}

	int SetRotX(float x) {_transformation.rotation.x = x; return 0;}
	int SetRotY(float y) {_transformation.rotation.y = y; return 0;}
	int SetRotZ(float z) {_transformation.rotation.z = z; return 0;}

	float RotX() {return _transformation.rotation.x;}
	float RotY() {return _transformation.rotation.y;}
	float RotZ() {return _transformation.rotation.z;}

	int ResetToNodeVector();
	int SetToNodeVector(EuclideanVector2D moveVec) {_toNodeVec = moveVec; return 0;}
	EuclideanVector2D GetToNodeVector() {return _toNodeVec;}

	int ResetMovementVector();
	int SetMovementVector(EuclideanVector2D moveVec) {_movementVec = moveVec; return 0;}
	EuclideanVector2D GetMovementVector() {return _movementVec;}
	int TurnMovementVector(bool positive, bool collision) {if (positive) _movementVec.direction += 5.0f; else _movementVec.direction -= 5.0f; return 0;}

	//Accessors
	EntityEventQueue* GetEventQueue() {return _entityEventQueue;}
	EntityProcessEventQueue* GetProcessEventQueue() {return _entityProcessEventQueue;}

	int SetEntityStatus(int status) {_entityStatus = status; return 0;}
	int GetEntityStatus() {return _entityStatus;}

	float GetAnimationSeconds() {return ((float)SDL_GetTicks() / 1000.0f) - _animationSeconds;}

	int SetIsMouseOver(bool flag) {_isMouseOver = flag; return 0;}
	bool IsMouseOver() {return _isMouseOver;}
	int SetIsSelected(bool flag) {_isSelected = flag; return 0;}
	bool IsSelected() {return _isSelected;}

private:
	friend class EntityBase;

	//Pointer to the instance creator
	EntityBase* _entityBase;

	EntityStats* _entityStats;

	//Transformation of the entity
	Transformation _transformation;

	//Vectors for movement
	EuclideanVector2D _toNodeVec;
	EuclideanVector2D _movementVec;

	//Event queue
	EntityEventQueue* _entityEventQueue;

	//Process event queue
	EntityProcessEventQueue * _entityProcessEventQueue;

	int _entityStatus;

	float _animationSeconds;

	bool _isMouseOver;
	bool _isSelected;

	float _radius;
};

#endif
