#ifndef _MM_PAGE_H
#define _MM_PAGE_H

#include <vector>

#include "MainMenu/UIElements/MMView.h"

#include "MainMenu/Menu.h"

class MMPage
{
public:
	MMPage();
	~MMPage();

	virtual int Init();
	int Preprocess(Menu* menu);
	int Render(Menu* menu);
	int HandleButtons(Menu* menu, int* outPage);
	virtual int PerformAction(Menu* menu, int i);
	virtual int PerformView(Menu* menu, int i);
	virtual int PerformPage(Menu* menu, int i);
protected:
	std::vector<MMView*> _viewVector;

	bool _needPreprocess;
};
#endif
