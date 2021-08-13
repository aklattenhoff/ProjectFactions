#ifndef _RS_GUI_TEXT_H
#define _RS_GUI_TEXT_H

#include <string>

namespace RS
{
class GUIText
{
public:
	static const int FONT_ARIAL_BOLD = 0;
public:
	GUIText();
	~GUIText();

	int SetText(int x, int y, std::string str, int height = 12, int font = FONT_ARIAL_BOLD);
	int SetString(std::string str);

	int GetX();
	int GetY();
	std::string GetString();
	int GetHeight();
	int GetFont();
private:
	int _x;
	int _y;
	std::string _string;
	int _height;
	int _font;
};
}
#endif
