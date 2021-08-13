#ifndef _RS_GUI_TEXTURE_H
#define _RS_GUI_TEXTURE_H

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace RS
{
class GUITexture
{
public:
	GUITexture();
	~GUITexture();

	int SetPoints(int x1, int y1, int x2, int y2, float depth = 0.1);
	int SetTexture(unsigned int tex);

	glm::vec4 GetPoints();
	float GetDepth();
	unsigned int GetTexture();
private:
	glm::vec4 _points;
	float _depth;
	unsigned int _texture;
};
}
#endif
