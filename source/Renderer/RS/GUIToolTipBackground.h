#ifndef _RS_GUI_TOOLTIP_BACKGROUND_H
#define _RS_GUI_TOOLTIP_BACKGROUND_H

#include <string>
#include <vector>

namespace RS
{
class GUIToolTipBackground
{
public:
	struct BorderBackground
	{
		unsigned int _backTex;
		unsigned int _cornerTexNW;
		unsigned int _cornerTexNE;
		unsigned int _cornerTexSE;
		unsigned int _cornerTexSW;
		unsigned int _edgeTexN;
		unsigned int _edgeTexS;
		unsigned int _edgeTexE;
		unsigned int _edgeTexW;
	};
public:
	GUIToolTipBackground();
	~GUIToolTipBackground();

	int SetToolTipBackgroundTextures(unsigned int back, unsigned int corNW, unsigned int corNE, unsigned int corSE, unsigned int corSW,
										unsigned int edgeN, unsigned int edgeS, unsigned int edgeE, unsigned int edgeW);
	int SetToolTipBackgroundTextures(BorderBackground back);
	int SetBorderWidth(int width);

	int GetBorderWidth();
	BorderBackground GetBorderBackground();
private:
	BorderBackground _borderBackground;
	int _borderWidth;
};
}
#endif
