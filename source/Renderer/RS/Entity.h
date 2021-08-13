#ifndef _RS_ENTITY_H
#define _RS_ENTITY_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Model;
class Player;
class EntityBase;

namespace RS
{
class Entity
{
public:
	Entity();
	~Entity();

	int SetModel(Model* model, glm::vec3 pos, float rot = 0.0f);
	int SetAnimation(int animIndex, float animTime);
	int SetPlayer(Player* player);
	int SetEntityBase(EntityBase* entBase);

	Model* GetModel();
	glm::vec3 GetPosition();
	float GetRotation();
	int GetAnimationIndex();
	float GetAnimationTime();
	Player* GetPlayer();
	EntityBase* GetEntityBase();
private:
	Model* _model;
	glm::vec3 _position;
	float _rotation;
	int _animationIndex;
	float _animationTime;
	Player* _player;
	EntityBase* _entityBase;
};
}
#endif
