#include "GUIToolTip.h"

RS::GUIToolTip::GUIToolTip()
{
}

RS::GUIToolTip::~GUIToolTip()
{
}

int RS::GUIToolTip::SetToolTip(int x, int y, std::string firstLine, int height)
{
	_x = x;
	_y = y;
	if (_stringVector.size() < 1)
		_stringVector.push_back(firstLine);
	else
		_stringVector[0] = firstLine;

	_height = height;

	return 0;
}

int RS::GUIToolTip::SetString(int line, std::string strLine)
{
	if ((int)_stringVector.size() < line+1)
		_stringVector.resize(line+1);

	_stringVector[line] = strLine;

	return 0;
}

int RS::GUIToolTip::SetX(int x)
{
	_x = x;

	return 0;
}

int RS::GUIToolTip::SetY(int y)
{
	_y = y;

	return 0;
}

int RS::GUIToolTip::GetX()
{
	return _x;
}

int RS::GUIToolTip::GetY()
{
	return _y;
}

std::string RS::GUIToolTip::GetString(int line)
{
	if ((int)_stringVector.size() < line+1)
		return 0;

	return _stringVector[line];
}

int RS::GUIToolTip::GetNumberOfLines()
{
	return (int)_stringVector.size();
}

int RS::GUIToolTip::GetHeight()
{
	return _height;
}
