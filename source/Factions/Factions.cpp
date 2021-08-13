#include "Factions.h"

#include "Texturer/Texturer.h"
#include "Renderer/Renderer.h"

#include "Parser/Parser.h"
#include "Parser/FactionsXML.h"

const char* FACTIONS_XML_FILENAME = "factions.xml";

Factions::Factions()
{
	_mainMenu = NULL;
	window = NULL;
	_game = NULL;
	_network = NULL;
	_parser = NULL;
}

Factions::~Factions()
{
	delete _mainMenu;
	delete _game;
	SDL_GL_DeleteContext(glContext);
	if (window)
		SDL_DestroyWindow(window);
	SDL_Quit();
	delete _network;
	delete _parser;
}

int Factions::ParseMainXML()
{
	_parser = new Parser();
	_parser->Init();
	_parser->Parse(FACTIONS_XML_FILENAME);

	return 0;
}

int Factions::CreateWindowAndGLContext()
{
	//Initialize SDL
	DebugOutput("Initializing SDL...");
	SDL_Init(SDL_INIT_EVERYTHING);
	DebugOutput("done\n");

	//Set GL double buffer and depth size
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Set the depth buffer and stencil buffer size
	//NOTE: The depth and stencil buffer share the same 32 bit buffer
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Create the window with dimensions equal to what was parsed from the factions.xml file
	DebugOutput("Creating window...");
	window = SDL_CreateWindow("Project Factions", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _parser->GetFactionsXML()->GetScreenWidth(), _parser->GetFactionsXML()->GetScreenHeight(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (_parser->GetFactionsXML()->GetFullScreen() != 0 ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE));
	DebugOutput("done\n");
	DebugOutput("Creating GL context...");
	glContext = SDL_GL_CreateContext(window);
	DebugOutput("done\n");

	//DEP: All openGL settings are moved to Phage::Scene

	//Output the OpenGL version to debug console
	/*int majVer = -1;
	int minVer = -1;
	int profile = -1;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majVer);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minVer);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
	DebugOutput("GL Version: %i.%i prof: %i\n", majVer, minVer, profile);*/

	//Set V-Sync
	//if (SDL_GL_SetSwapInterval(_parser->GetFactionsXML()->GetVSync()) == 0)
		//DebugOutput("V-Sync set\n");
	//else
		//DebugOutput("V-Sync not supported\n");

	return 0;
}

int Factions::InitGLExtensions()
{
	//loadExtensions
	DebugOutput("Loading GL Extensions\n");
	LoadGLExtensions();

	return 0;
}

int Factions::Init()
{
	//Initialize the texturer
	DebugOutput("Creating Textuerer...");
	Texturer::CreateSingleton();
	DebugOutput("done\n");

	//Initialize the renderer
	DebugOutput("Creating Renderer...");
	Renderer::CreateSingleton();
	//RendererSingleton->InitGraphics(_parser);
	RendererSingleton->SetWindow(window);
	if (RendererSingleton->CreateScene() == -1)
		return -1;
	RendererSingleton->SetResolution(_parser->GetFactionsXML()->GetScreenWidth(), _parser->GetFactionsXML()->GetScreenHeight());
	DebugOutput("done\n");

	//Create objects to send to the Main Menu but don't initialize them
	_network = new Network();
	_game = new Game();
	//DebugOutput("Initializing camera...\n");
	//_game->InitCamera(factionsXMLParser->GetScreenResolution());
	//DebugOutput("...Done Initializing camera\n");

	DebugOutput("Creating game...\n");
	_game->Init();
	_game->SetWindow(window);
	_game->SetParser(_parser);
	//_game->SetFactionList(factionList);
	//_game->SetBottomBarFile(factionsXMLParser->GetBottomBar());
	_game->SetBottomBarFile(_parser->GetFactionsXML()->GetBottomBarFileName().c_str());
	DebugOutput("...Done Creating game\n");

	//Parse all XMLs for all factions
	//_game->ParseFactions(factionsXMLParser->GetFactionListFileName());
	_game->ParseFactions(_parser->GetFactionsXML()->GetFactionListFileName().c_str());

	//_game->CreateNewGame();

	_mainMenu = new MainMenu();

	return 0;
}

int Factions::StartProgram()
{
	//Initialize the Main Menu and run
	//_mainMenu->InitCamera(factionsXMLParser->GetScreenResolution());
	//_mainMenu->InitCamera(glm::vec2(_parser->GetFactionsXML()->GetScreenWidth(), _parser->GetFactionsXML()->GetScreenHeight()));
	_mainMenu->SetWindow(window);
	_mainMenu->SetNetwork(_network);
	_mainMenu->SetGame(_game);
	_mainMenu->Init();

	//SoLoud::Wav music;
	//music.load("music/Resilience.ogg");
	//_soloud->play(music);

	int returnValue = _mainMenu->Run();

	return 0;
}

int Factions::Frame()
{

	/*if (returnValue == 1)
	{
		//NOTE: This will only happen when the user initializes a game
		//NOTE: The Game object has it's own program loop and the object is deleted when Factions is deleted
		_game = new Game();

		DebugOutput("Initializing camera...\n");
		game->InitCamera(factionsXMLParser->GetScreenResolution());
		DebugOutput("...Done Initializing camera\n");

		DebugOutput("Creating game...\n");
		game->Init();
		game->SetWindow(window);
		game->SetFactionList(factionList);
		game->SetBottomBarFile(factionsXMLParser->GetBottomBar());
		DebugOutput("...Done Creating game\n");

		//Parse all XMLs for all factions
		game->ParseFactions(factionsXMLParser->GetFactionListFileName());

		game->CreateNewGame();

		DebugOutput("Running game...\n");
		int returnValue = game->Run();
		DebugOutput("...Done Running game\n");

		return returnValue;
	}*/

	return 0;
}
