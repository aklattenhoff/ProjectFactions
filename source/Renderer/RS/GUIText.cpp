#include "GUIText.h"

RS::GUIText::GUIText()
{

}

RS::GUIText::~GUIText()
{

}

int RS::GUIText::SetText(int x, int y, std::string str, int height, int font)
{
	_x = x;
	_y = y;
	_string = str;
	_height = height;
	_font = font;

	return 0;
}

int RS::GUIText::SetString(std::string str)
{
	_string = str;

	return 0;
}

int RS::GUIText::GetX()
{
	return _x;
}

int RS::GUIText::GetY()
{
	return _y;
}

std::string RS::GUIText::GetString()
{
	return _string;
}

int RS::GUIText::GetHeight()
{
	return _height;
}

int RS::GUIText::GetFont()
{
	return _font;
}
