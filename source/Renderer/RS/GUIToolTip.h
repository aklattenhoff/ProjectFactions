#ifndef _RS_GUI_TOOLTIP_H
#define _RS_GUI_TOOLTIP_H

#include <string>
#include <vector>

namespace RS
{
class GUIToolTip
{
public:
	GUIToolTip();
	~GUIToolTip();

	int SetToolTip(int x, int y, std::string firstLine, int height = 12);
	int SetString(int line, std::string strLine);

	int SetX(int x);
	int SetY(int y);
	int GetX();
	int GetY();
	std::string GetString(int line);
	int GetNumberOfLines();
	int GetHeight();
private:
	int _x;
	int _y;
	std::vector<std::string> _stringVector;
	int _height;
};
}
#endif
