#include "MMView.h"

MMView::MMView()
{
}

MMView::~MMView()
{
	for (unsigned int i = 0;i < _buttonVector.size();i ++)
		delete _buttonVector[i];
}

int MMView::Init()
{
	return 0;
}

int MMView::Preprocess(Menu* menu)
{
	for (unsigned int i = 0;i < _buttonVector.size();i ++)
		_buttonVector[i]->Preprocess(menu);

	return 0;
}

int MMView::Render(Menu* menu)
{
	for (unsigned int i = 0;i < _buttonVector.size();i ++)
	{
		_buttonVector[i]->Render(menu);
	}

	return 0;
}

MMButton* MMView::GetButtonAtPosition(glm::vec2 pos)
{
	for (unsigned int i = 0;i < _buttonVector.size();i ++)
	{

		glm::vec2 buttonPosition = _buttonVector[i]->GetTruePosition();
		glm::vec2 buttonSize = _buttonVector[i]->GetTrueSize();

		glm::vec2 deltaPosition = pos - buttonPosition;

		if (deltaPosition.x > 0.0f && deltaPosition.y > 0.0f && deltaPosition.x < buttonSize.x && deltaPosition.y < buttonSize.y)
		{
			return _buttonVector[i];
		}
	}

	return NULL;
}

MMView::MMButtonVector MMView::GetButtonVector()
{
	return _buttonVector;
}

int MMView::AddButton(MMButton* button)
{
	_buttonVector.push_back(button);

	return 0;
}
