#include "MMPage.h"

MMPage::MMPage()
{
}

MMPage::~MMPage()
{
	for (unsigned int i = 0;i < _viewVector.size();i ++)
		delete _viewVector[i];
}

int MMPage::Init()
{
	_needPreprocess = false;

	return 0;
}

int MMPage::Preprocess(Menu* menu)
{
	for (unsigned int i = 0;i < _viewVector.size();i ++)
		_viewVector[i]->Preprocess(menu);

	return 0;
}

int MMPage::Render(Menu* menu)
{
	if (_needPreprocess)
	{
		Preprocess(menu);
		_needPreprocess = false;
	}

	for (unsigned int i = 0;i < _viewVector.size();i ++)
		_viewVector[i]->Render(menu);

	return 0;
}

int MMPage::HandleButtons(Menu* menu, int* outPage)
{
	if (!menu->GetInput()->GetMouseInput(INPUT_MOUSE_PRESSED, INPUT_MOUSE_LEFTBUTTON))
		return 0;

	(*outPage) = -1;

	for (unsigned int i = 0;i < _viewVector.size();i ++)
	{
		MMButton* button = _viewVector[i]->GetButtonAtPosition(menu->GetInput()->GetMousePosition());
		if (button)
		{
			if (button->GetClickFlag() & MMButton::CLICK_VIEW)
				PerformView(menu, button->GetClickView());
			if (button->GetClickFlag() & MMButton::CLICK_ACTION)
				PerformAction(menu, button->GetClickAction());
			if (button->GetClickFlag() & MMButton::CLICK_PAGE)
			{
				PerformPage(menu, button->GetClickPage());
				(*outPage) = button->GetClickPage();
			}

			return 0;
		}
	}

	PerformView(menu, 0);
	PerformAction(menu, 0);
	PerformPage(menu, 0);

	return 0;
}

int MMPage::PerformAction(Menu* menu, int i)
{

	return 0;
}

int MMPage::PerformView(Menu* menu, int i)
{

	return 0;
}

int MMPage::PerformPage(Menu* menu, int i)
{

	return 0;
}
