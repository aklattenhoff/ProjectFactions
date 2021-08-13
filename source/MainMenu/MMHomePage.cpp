#include "MMHomePage.h"

#include "MainMenu/UIElements/MMView.h"

#include "Renderer/Renderer.h"

MMHomePage::MMHomePage()
{
}

MMHomePage::~MMHomePage()
{
}

int MMHomePage::Init()
{
	//Create the view
	_viewVector.push_back(new MMView());
	_viewVector.back()->Init();

	//Set the buttons
	MMButton* button;

	button = new MMButton();
	button->Init(glm::vec2(4, 16), glm::vec2(16, 8), "Solo Game");
	button->AddClick(MMButton::CLICK_ACTION, 1);
	_viewVector.back()->AddButton(button);

	button = new MMButton();
	button->Init(glm::vec2(4, 24), glm::vec2(16, 8), "Multiplayer");
	button->AddClick(MMButton::CLICK_VIEW, 1);
	_viewVector.back()->AddButton(button);

	button = new MMButton();
	button->Init(glm::vec2(4, 32), glm::vec2(16, 8), "Help");
	_viewVector.back()->AddButton(button);

	MMPage::Init();

	return 0;
}

int MMHomePage::PerformAction(Menu* menu, int i)
{
	switch (i)
	{
		case 0:
			break;
		case 1:
			//menu->GetGame()->InitCamera(menu->GetCamera()->GetScreenResolution());
			menu->GetGame()->InitCamera(glm::vec2(RendererSingleton->GetScreenWidth(), RendererSingleton->GetScreenHeight()));
			menu->GetGame()->CreateNewGame();
			menu->GetGame()->Run();
			SDL_ShowCursor(1);
			break;
	}

	return 0;
}

int MMHomePage::PerformView(Menu* menu, int i)
{
	switch (i)
	{
		case 0:
			while (_viewVector.size() > 1)
			{
				delete _viewVector.back();
				_viewVector.pop_back();
			}
			break;
		case 1:
			while (_viewVector.size() > 1)
			{
				delete _viewVector.back();
				_viewVector.pop_back();
			}
			_viewVector.push_back(new MMView());
			_viewVector.back()->Init();

			MMButton* button;

			button = new MMButton();
			button->Init(glm::vec2(20, 24), glm::vec2(16, 8), "LAN");
			_viewVector.back()->AddButton(button);
			_needPreprocess = true;

			break;
	}

	return 0;
}
