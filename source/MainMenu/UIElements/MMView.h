#ifndef _MM_VIEW_H
#define _MM_VIEW_H

#include <vector>

#include "MainMenu/UIElements/MMButton.h"

#include "MainMenu/Menu.h"

class MMView
{
public:
	typedef std::vector<MMButton*> MMButtonVector;
public:
	MMView();
	~MMView();

	int Init();
	int Preprocess(Menu* menu);
	int Render(Menu* menu);
	MMButton* GetButtonAtPosition(glm::vec2 pos);
	MMButtonVector GetButtonVector();
	int AddButton(MMButton* button);
private:
	std::vector<MMButton*> _buttonVector;
};
#endif
