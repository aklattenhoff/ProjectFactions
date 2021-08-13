#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

int Input::Init()
{
	memset(keyboardDown, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);
	memset(keyboardPressed, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);
	memset(keyboardRelease, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);
	memset(keyboardPressedEvent, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);

	mouseMotion = false;
	mousePosition = glm::vec2(0.0f);
	mouseWheel = glm::ivec2(0);

	memset(mouseDown, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);
	memset(mousePressed, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);
	memset(mouseRelease, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);
	memset(mousePressedEvent, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);

	windowSizeChanged = false;
	windowSize = glm::ivec2(0);

	quit = false;

	return 0;
}

int Input::ResetSingleFrameInput()
{
	memset(keyboardPressed, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);
	memset(keyboardRelease, false, sizeof(bool)*INPUT_KEYBOARD_MAX_KEYS);

	mouseMotion = false;
	mouseWheel.x = 0;
	mouseWheel.y = 0;

	memset(mousePressed, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);
	memset(mouseRelease, false, sizeof(bool)*INPUT_MOUSE_MAX_BUTTONS);

	windowSizeChanged = false;

	return 0;
}

bool Input::GetKeyboardInput(const int type, int key)
{
	if (type == INPUT_KEYBOARD_DOWN)
		return keyboardDown[SDL_GetScancodeFromKey(key)];
	else if (type == INPUT_KEYBOARD_PRESSED)
		return keyboardPressed[SDL_GetScancodeFromKey(key)];
	else if (type == INPUT_KEYBOARD_RELEASE)
		return keyboardRelease[SDL_GetScancodeFromKey(key)];

	return false;
}

bool Input::GetMouseInput(const int type, int button)
{
	if (type == INPUT_MOUSE_DOWN)
		return mouseDown[button];
	else if (type == INPUT_MOUSE_PRESSED)
		return mousePressed[button];
	else if (type == INPUT_MOUSE_RELEASE)
		return mouseRelease[button];

	return false;
}

glm::vec2 Input::GetMousePosition()
{
	return mousePosition;
}

bool Input::GetMouseMotion()
{
	return mouseMotion;
}

glm::ivec2 Input::GetMouseWheel()
{
	return mouseWheel;
}

bool Input::GetWindowSizeChange()
{
	return windowSizeChanged;
}

glm::ivec2 Input::GetWindowSize()
{
	return windowSize;
}

bool Input::GetQuit()
{
	return quit;
}

int Input::ConsumeMouseInput(const int type, int button)
{
	if (type == INPUT_MOUSE_DOWN)
		mouseDown[button] = false;
	else if (type == INPUT_MOUSE_PRESSED)
		mousePressed[button] = false;
	else if (type == INPUT_MOUSE_RELEASE)
		mouseRelease[button] = false;

	return 0;
}
