#include "MMButton.h"

#include "Renderer/Renderer.h"

MMButton::MMButton()
{
	_backgroundRectangle = NULL;
}

MMButton::~MMButton()
{
	delete _backgroundRectangle;
	delete _text;
}

int MMButton::Init()
{
	//_backgroundRectangle = new GUIRectangle();
	//_backgroundRectangle->SetPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	//_text = new GUIText();
	//_text->Init("arialbd.ttf", 16);

	_rsRectangle.SetPoints(0, 0, 0, 0);
	_rsText.SetText(0, 0, "");

	return 0;
}

int MMButton::Init(glm::vec2 pos, glm::vec2 size, std::string str)
{
	Init();

	SetPosition(pos);
	SetSize(size);
	SetText(str);

	return 0;
}

int MMButton::Preprocess(Menu* menu)
{
	_truePosition.x = (_position.x * (float)RendererSingleton->GetScreenWidth()) / 100.0f;
	_truePosition.y = (_position.y * (float)RendererSingleton->GetScreenHeight()) / 100.0f;

	_trueSize.x = (_size.x * (float)RendererSingleton->GetScreenWidth()) / 100.0f;
	_trueSize.y = (_size.y * (float)RendererSingleton->GetScreenHeight()) / 100.0f;

	//_backgroundRectangle->SetPoints(glm::vec4(_truePosition.x, _truePosition.y, _truePosition.x+_trueSize.x, _truePosition.y+_trueSize.y));

	//_text->Destroy();
	//delete _text;
	//_text = new GUIText();
	//_text->Init("arialbd.ttf", (int)(_trueSize.y/4.0f));
	float approxTextWidth = (float)_textString.size();
	approxTextWidth = approxTextWidth * (3.0f*_trueSize.y/4.0f)/4.0f;
	_textOffset = (int)((_trueSize.x - approxTextWidth) / 2.0f);

	_rsRectangle.SetPoints((int)_truePosition.x, (int)_truePosition.y, (int)(_truePosition.x+_trueSize.x), (int)(_truePosition.y+_trueSize.y));

	return 0;
}

int MMButton::Render(Menu* menu)
{
	//menu->GetCamera()->GetShader()->UseProgram(SHADER_NO_TEXTURE);
	//menu->GetCamera()->UpdateMatrix(SHADER_NO_TEXTURE);

	//menu->GetCamera()->GetShader()->SetColor(0.75f, 0.6f, 0.6f);
	//_backgroundRectangle->Draw();

	//menu->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
	//menu->GetCamera()->UpdateMatrix(SHADER_DEFAULT);

	//_text->Draw((int)_truePosition.x + _textOffset, (int)_truePosition.y + (int)((_trueSize.y - _trueSize.y/4.0f) / 2.0f), _textString, menu->GetCamera());

	RendererSingleton->AddGUIRectangle(_rsRectangle);

	_rsText.SetText((int)_truePosition.x + _textOffset, (int)_truePosition.y + (int)((_trueSize.y - _trueSize.y/4.0f) / 2.0f), _textString, (int)(_trueSize.y/4.0f));
	RendererSingleton->AddGUIText(_rsText);

	return 0;
}

int MMButton::SetText(std::string str)
{
	_textString = str;

	return 0;
}

std::string MMButton::GetText()
{
	return _textString;
}

int MMButton::SetPosition(glm::vec2 pos)
{
	_position = pos;

	return 0;
}

glm::vec2 MMButton::GetPosition()
{
	return _position;
}

glm::vec2 MMButton::GetTruePosition()
{
	return _truePosition;
}

int MMButton::SetSize(glm::vec2 siz)
{
	_size = siz;

	return 0;
}

glm::vec2 MMButton::GetSize()
{
	return _size;
}

glm::vec2 MMButton::GetTrueSize()
{
	return _trueSize;
}

int MMButton::SetClickFlag(int flag)
{
	_clickFlag = flag;

	return 0;
}

int MMButton::AddClickFlag(int flag)
{
	_clickFlag |= flag;

	return 0;
}

int MMButton::AddClick(int flag, int value)
{
	switch (flag)
	{
		case CLICK_PAGE: _page = value; AddClickFlag(CLICK_PAGE); break;
		case CLICK_VIEW: _view = value; AddClickFlag(CLICK_VIEW); break;
		case CLICK_ACTION: _action = value; AddClickFlag(CLICK_ACTION); break;
	}

	return 0;
}

int MMButton::GetClickFlag()
{
	return _clickFlag;
}

int MMButton::GetClickPage()
{
	return _page;
}

int MMButton::GetClickView()
{
	return _view;
}

int MMButton::GetClickAction()
{
	return _action;
}
