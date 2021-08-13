#ifndef _RS_GUI_RECTANGLE_H
#define _RS_GUI_RECTANGLE_H

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace RS
{
class GUIRectangle
{
public:
	GUIRectangle();
	~GUIRectangle();

	int SetPoints(int x1, int y1, int x2, int y2);

	glm::vec4 GetPoints();
private:
	glm::vec4 _points;
};
}
#endif
