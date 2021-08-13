#include "MainMenu.h"

#include "MainMenu/MMHomePage.h"

#include "Renderer/Renderer.h"
#include "Renderer/RS/GUIText.h"

MainMenu::MainMenu()
{
	_input = NULL;
	//_camera = NULL;
	_guiText = NULL;
	_menu = NULL;
	_mmLANLobby = NULL;
	_mmLANHost = NULL;
	_mmPage = NULL;
}

MainMenu::~MainMenu()
{
	delete _input;
	//delete _camera;
	delete _guiText;
	delete _menu;
	delete _mmLANLobby;
	delete _mmLANHost;
	delete _mmPage;
}

int MainMenu::InitCamera(glm::vec2 screenResolution)
{
	//Creates the Camera object and passes the screen resolution
	/*_camera = new Camera();
	_camera->SetScreenResolution(screenResolution);

	_camera->setCameraPosition(0.0f, 0.0f, 0.0f);

	_camera->GetShader()->UseProgram(SHADER_DEFAULT);
	_camera->GetMVPMatrix()->SetProjectionMatrixType(ORTHO);
	_camera->UpdateMatrix(SHADER_DEFAULT);
	_camera->GetShader()->SetLighting(0);*/

	return 0;
}

int MainMenu::Init()
{
	_input = new Input();
	_input->Init();

	_guiText = new GUIText();
	_guiText->Init("arialbd.ttf", 16);

	_textureCursor.file = "art/cursor.png";
	_textureCursor.yflip = false;
	_textureCursor.mipmap = false;
	TexturerSingleton->LoadTexture(&_textureCursor, 1);

	_menu = new Menu();
	_menu->Init();
	//_menu->SetCamera(_camera);
	_menu->SetInput(_input);
	_menu->SetNetwork(_network);
	_menu->SetGame(_game);

	_mmLANLobby = new MMLANLobby();
	_mmLANLobby->Init();

	_mmLANHost = new MMLANHost();
	_mmLANHost->Init();

	_mmPage = new MMHomePage();
	_mmPage->Init();
	_mmPage->Preprocess(_menu);

	_page = 0;

	return 0;
}

int MainMenu::SetWindow(SDL_Window* w)
{
	_window = w;

	return 0;
}

int MainMenu::SetNetwork(Network* n)
{
	_network = n;

	return 0;
}

int MainMenu::SetGame(Game* g)
{
	_game = g;

	return 0;
}

int MainMenu::Run()
{
	_network->CreateJoinSocket();
	_network->StartJoinBroadcast();

	//TODO: Remove this as it shouldn't be set to zero before this
	SDL_ShowCursor(0);

	int returnValue = 0;

	//Main menu loop
	bool done = false;
	while (!done)
	{
		returnValue = Frame();
		done = !(returnValue == 0);

		//Frames per second calculator
		FPS += 1.0f;
		_menu->SetTime(SDL_GetTicks());
		if (_menu->GetTime()-ticks > 1000)
		{
			FPS /= ((float)_menu->GetTime()-ticks)/1000.0f;
			ticks = _menu->GetTime();
			FPS = 0.0f;
		}
	}

	_network->DestroyJoinSocket();

	return returnValue;
}

int MainMenu::Frame()
{
	//Renderer TEST
	//RS::GUIText guiTextRS;
	//guiTextRS.SetText(10, 10, "ASDASDASD");
	//RendererSingleton->AddGUIText(guiTextRS);

	int returnValue = 0;

	//_camera->GetMVPMatrix()->identity(VIEW);
	//_camera->GetMVPMatrix()->identity(MODEL);
	//_camera->UpdateMatrix(SHADER_DEFAULT);

	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	int pageReturnValue = 0;

	TexturerSingleton->RunTextureWork(1);

	_mmPage->Render(_menu);

	_mmPage->HandleButtons(_menu, &pageReturnValue);

	//This may need to be added into a separate class
	_rsGUITextureCursor.SetTexture(_textureCursor.id);
	int mX, mY;
	SDL_GetMouseState(&mX, &mY);
	_rsGUITextureCursor.SetPoints(mX, mY, mX+32, mY+32, 0.999f);
	RendererSingleton->AddGUITexture(_rsGUITextureCursor);


	//Create new page based on the return value

	RendererSingleton->Render();

	//SDL_GL_SwapWindow(_window);

	_input->ResetSingleFrameInput();

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				_input->mouseDown[sdlEvent.button.button-1] = true;
				if (!_input->mousePressedEvent[sdlEvent.button.button-1])
					_input->mousePressed[sdlEvent.button.button-1] = true;
				_input->mousePressedEvent[sdlEvent.button.button-1] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_input->mouseDown[sdlEvent.button.button-1] = false;
				_input->mouseRelease[sdlEvent.button.button-1] = true;
				_input->mousePressedEvent[sdlEvent.button.button-1] = false;
				break;
			case SDL_MOUSEMOTION:
				_input->mousePosition.x = (float)sdlEvent.motion.x;
				_input->mousePosition.y = (float)sdlEvent.motion.y;
				break;
			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						//_camera->SetScreenResolution(glm::vec2(sdlEvent.window.data1, sdlEvent.window.data2));
						RendererSingleton->SetResolution(sdlEvent.window.data1, sdlEvent.window.data2);
						_mmPage->Preprocess(_menu);
						glViewport(0, 0, sdlEvent.window.data1, sdlEvent.window.data2);
						break;
				}
				break;
			case SDL_KEYDOWN:
				_input->keyboardDown[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				if (!_input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)])
					_input->keyboardPressed[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				_input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				break;
			case SDL_KEYUP:
				_input->keyboardDown[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = false;
				_input->keyboardRelease[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = true;
				_input->keyboardPressedEvent[SDL_GetScancodeFromKey(sdlEvent.key.keysym.sym)] = false;
				break;
			case SDL_QUIT:
				returnValue = -1;
				break;
		}
	}

	return returnValue;
}
