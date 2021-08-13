#ifndef _MM_HOME_PAGE_H
#define _MM_HOME_PAGE_H

#include <vector>

#include "MainMenu/UIElements/MMPage.h"

class MMHomePage : public MMPage
{
public:
	MMHomePage();
	~MMHomePage();

	int Init();
	int PerformAction(Menu* menu, int i);
	int PerformView(Menu* menu, int i);
private:
	
};
#endif
