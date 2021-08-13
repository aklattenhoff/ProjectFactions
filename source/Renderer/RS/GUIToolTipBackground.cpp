#include "GUIToolTipBackground.h"

RS::GUIToolTipBackground::GUIToolTipBackground()
{
}

RS::GUIToolTipBackground::~GUIToolTipBackground()
{
}

int RS::GUIToolTipBackground::SetToolTipBackgroundTextures(unsigned int back, unsigned int corNW, unsigned int corNE, unsigned int corSE,
										unsigned int corSW, unsigned int edgeN, unsigned int edgeS, unsigned int edgeE, unsigned int edgeW)
{
	_borderBackground._backTex = back;
	_borderBackground._cornerTexNW = corNW;
	_borderBackground._cornerTexNE = corNE;
	_borderBackground._cornerTexSE = corSE;
	_borderBackground._cornerTexSW = corSW;
	_borderBackground._edgeTexN = edgeN;
	_borderBackground._edgeTexS = edgeS;
	_borderBackground._edgeTexE = edgeE;
	_borderBackground._edgeTexW = edgeW;

	return 0;
}

int RS::GUIToolTipBackground::SetToolTipBackgroundTextures(RS::GUIToolTipBackground::BorderBackground back)
{
	_borderBackground = back;

	return 0;
}

int RS::GUIToolTipBackground::SetBorderWidth(int width)
{
	_borderWidth = width;

	return 0;
}

int RS::GUIToolTipBackground::GetBorderWidth()
{
	return _borderWidth;
}

RS::GUIToolTipBackground::BorderBackground RS::GUIToolTipBackground::GetBorderBackground()
{
	return _borderBackground;
}
