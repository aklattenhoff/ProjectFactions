#ifndef _MM_BUTTON_H
#define _MM_BUTTON_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "MainMenu/Menu.h"

#include "Game/GUI/GUIRectangle.h"
#include "Game/GUI/GUIText.h"

#include "Renderer/RS/GUIRectangle.h"
#include "Renderer/RS/GUIText.h"

class MMButton
{
public:
	static const int CLICK_PAGE		=		0x00000001;
	static const int CLICK_VIEW		=		0x00000002;
	static const int CLICK_ACTION	=		0x00000004;
public:
	MMButton();
	~MMButton();

	int Init();
	int Init(glm::vec2 pos, glm::vec2 size, std::string str);
	int Preprocess(Menu* menu);
	int Render(Menu* menu);

	int SetText(std::string str);
	std::string GetText();

	int SetPosition(glm::vec2 pos);
	glm::vec2 GetPosition();
	glm::vec2 GetTruePosition();

	int SetSize(glm::vec2 siz);
	glm::vec2 GetSize();
	glm::vec2 GetTrueSize();

	int SetClickFlag(int flag);
	int AddClickFlag(int flag);
	int AddClick(int flag, int value);
	int GetClickFlag();
	int GetClickPage();
	int GetClickView();
	int GetClickAction();
private:
	std::string _textString;
	int _textOffset;
	glm::vec2 _position;
	glm::vec2 _truePosition;
	glm::vec2 _size;
	glm::vec2 _trueSize;
	int _clickFlag;
	int _page;
	int _view;
	int _action;

	GUIRectangle* _backgroundRectangle;
	GUIText* _text;

	RS::GUIRectangle _rsRectangle;
	RS::GUIText _rsText;
};
#endif
