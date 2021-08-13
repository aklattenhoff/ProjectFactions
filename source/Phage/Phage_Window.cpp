#include "Phage_Window.h"

#include <SDL/SDL.h>

Phage::Window::Window()
{
}

Phage::Window::~Window()
{
}

int Phage::Window::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	return 0;
}

int Phage::Window::CreateSDLWindow(std::string title, int w, int h, bool fullscreen)
{
	_window = SDL_CreateWindow(title.c_str(), 400, 200, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (fullscreen == true ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE));

	return 0;
}

SDL_Window* Phage::Window::GetSDLWindow()
{
	return _window;
}
