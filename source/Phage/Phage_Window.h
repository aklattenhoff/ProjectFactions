#ifndef _PHAGE_WINDOW_H
#define _PHAGE_WINDOW_H

#include <stdlib.h>
#include <string>

struct SDL_Window;

namespace Phage
{
	class Window
	{
	public:
		Window();
		~Window();

		int Init();

		int CreateSDLWindow(std::string title, int w, int h, bool fullscreen = false);
		SDL_Window* GetSDLWindow();
	private:
		SDL_Window* _window;
	};
}

#endif
