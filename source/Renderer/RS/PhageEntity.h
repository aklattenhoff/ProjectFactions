#ifndef _RS_PHAGE_ENTITY_H
#define _RS_PHAGE_ENTITY_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class PhageEntity;
}
class Player;
class EntityBase;

namespace RS
{
	class Prop;
	class PhageEntity
	{
	public:
		PhageEntity();
		~PhageEntity();

		int SetModel(Phage::PhageEntity* phEnt, glm::vec3 pos, float rot = 0.0f);
		int SetAnimation(int animIndex, float animTime);
		int SetPlayer(Player* player);
		int SetEntityBase(EntityBase* entBase);

		Phage::PhageEntity* GetModel();
		glm::vec3 GetPosition();
		float GetRotation();
		int GetAnimationIndex();
		float GetAnimationTime();
		Player* GetPlayer();
		EntityBase* GetEntityBase();

		int AddProp(Prop prop);
		int PropSize();
		Prop PropAt(int index);
	private:
		Phage::PhageEntity* _phageEntity;
		glm::vec3 _position;
		float _rotation;
		int _animationIndex;
		float _animationTime;
		Player* _player;
		EntityBase* _entityBase;

		std::vector<Prop> _propVector;
	};
}
#endif
