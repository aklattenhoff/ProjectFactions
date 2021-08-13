#ifndef _RS_TERRAIN_DECAL_H
#define _RS_TERRAIN_DECAL_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace RS
{
class TerrainDecal
{
public:
	TerrainDecal();
	~TerrainDecal();

	int SetPoints(float x, float y, float w, float h, float depth = 0.1);
	int SetOrigin(float x, float y);
	int SetRotation(float rot);
	int SetTexture(unsigned int tex);
	int SetBumpTexture(unsigned int tex);

	glm::vec4 GetPoints();
	glm::vec2 GetOrigin();
	float GetRotation();
	float GetDepth();
	unsigned int GetTexture();
	unsigned int GetBumpTexture();
private:
	glm::vec4 _points;
	glm::vec2 _origin;
	float _rotation;
	float _depth;
	unsigned int _texture;
	unsigned int _bumpTexture;
};
}
#endif
