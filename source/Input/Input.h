#ifndef _INPUT_H
#define _INPUT_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadGLE.h"
#include "Debug/Debug.h"

const int INPUT_KEYBOARD_MAX_KEYS = 282;
const int INPUT_MOUSE_MAX_BUTTONS = 3;

const int INPUT_KEYBOARD_DOWN = 0;
const int INPUT_KEYBOARD_PRESSED = 1;
const int INPUT_KEYBOARD_RELEASE = 2;

const int INPUT_MOUSE_DOWN = 3;
const int INPUT_MOUSE_PRESSED = 4;
const int INPUT_MOUSE_RELEASE = 5;

const int INPUT_MOUSE_LEFTBUTTON = 0;
const int INPUT_MOUSE_MIDDLEBUTTON = 1;
const int INPUT_MOUSE_RIGHTBUTTON = 2;

class Input
{
public:
	Input();
	~Input();

	int Init();

	bool GetKeyboardInput(const int type, int key);
	bool GetMouseInput(const int type, int button);
	glm::vec2 GetMousePosition();
private:
	//This Input class is friends with the Game class so that only Game can set these values
	//Everyone else can use the get methods to retrieve the values
	friend class Game;
	friend class MainMenu;

	int ResetSingleFrameInput();

	bool keyboardDown[INPUT_KEYBOARD_MAX_KEYS];
	bool keyboardPressed[INPUT_KEYBOARD_MAX_KEYS];
	bool keyboardRelease[INPUT_KEYBOARD_MAX_KEYS];
	bool keyboardPressedEvent[INPUT_KEYBOARD_MAX_KEYS];

	glm::vec2 mousePosition;

	bool mouseDown[INPUT_MOUSE_MAX_BUTTONS];
	bool mousePressed[INPUT_MOUSE_MAX_BUTTONS];
	bool mouseRelease[INPUT_MOUSE_MAX_BUTTONS];
	bool mousePressedEvent[INPUT_MOUSE_MAX_BUTTONS];
};

#endif
