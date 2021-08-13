#ifndef _RS_GUI_LINE_RECT_H
#define _RS_GUI_LINE_RECT_H

#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace RS
{
class GUILineRect
{
public:
	GUILineRect();
	~GUILineRect();

	int SetPoints(int x1, int y1, int x2, int y2);
	int SetPoints(glm::vec4 ps);

	glm::vec4 GetPoints();
private:
	glm::vec4 _points;
};
}
#endif
