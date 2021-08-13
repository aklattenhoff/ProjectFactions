#ifndef _RS_PHAGE_TERRAIN_DECAL_H
#define _RS_PHAGE_TERRAIN_DECAL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Phage
{
	class PhageTerrainDecal;
}
class Player;
class EntityBase;

namespace RS
{
	class PhageTerrainDecal
	{
	public:
		PhageTerrainDecal();
		~PhageTerrainDecal();

		int SetTerrainDecal(Phage::PhageTerrainDecal* phTerrainDecal, glm::vec3 pos, float rot = 0.0f);
		int SetPlayer(Player* player);
		int SetEntityBase(EntityBase* entBase);

		Phage::PhageTerrainDecal* GetTerrainDecal();
		glm::vec3 GetPosition();
		float GetRotation();
		Player* GetPlayer();
		EntityBase* GetEntityBase();
	private:
		Phage::PhageTerrainDecal* _phageTerrainDecal;
		glm::vec3 _position;
		float _rotation;
		Player* _player;
		EntityBase* _entityBase;
	};
}
#endif
