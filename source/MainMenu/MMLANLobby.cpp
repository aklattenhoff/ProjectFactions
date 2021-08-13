#include "MMLANLobby.h"

#include "Game/Input/Input.h"
#include "Network/Network.h"

MMLANLobby::MMLANLobby()
{
	_guiTitleText = NULL;
	_guiText = NULL;
	_hoverRectangle = NULL;
	_selectedRectangle = NULL;
}

MMLANLobby::~MMLANLobby()
{
	delete _guiTitleText;
	delete _guiText;
	delete _hoverRectangle;
	delete _selectedRectangle;
}

int MMLANLobby::Init()
{
	_guiTitleText = new GUIText();
	_guiTitleText->Init("arialbd.ttf", 24);

	_guiText = new GUIText();
	_guiText->Init("arialbd.ttf", 16);

	_hoverRectangle = new GUIRectangle();
	_hoverRectangle->SetPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_hoverRectangle->SetDepth(1);

	_selectedRectangle = new GUIRectangle();
	_selectedRectangle->SetPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_selectedRectangle->SetDepth(0);

	_hoverUI = -1;
	_selectedUI = -1;

	_ticker = 0;
	_broadcastsSent = 0;

	return 0;
}

int MMLANLobby::Render(Menu* menu)
{
	//Spacing values
	float columnf = 420.0f;
	float textLengthf = 380.0f;
	int columni = (int)columnf;
	int textLengthi = (int)textLengthf;

	//Get inputs
	Input* input = menu->GetInput();
	glm::vec2 mousePos = input->GetMousePosition();

	//Draw rectangles
	/*menu->GetCamera()->GetShader()->UseProgram(SHADER_NO_TEXTURE);
	menu->GetCamera()->UpdateMatrix(SHADER_NO_TEXTURE);

	menu->GetCamera()->GetShader()->SetColor(0.5f, 0.5f, 0.5f);
	_hoverRectangle->Draw();

	menu->GetCamera()->GetShader()->SetColor(0.7f, 0.7f, 0.7f);
	_selectedRectangle->Draw();

	menu->GetCamera()->GetShader()->UseProgram(SHADER_DEFAULT);
	menu->GetCamera()->UpdateMatrix(SHADER_DEFAULT);

	//Reset hover rectangle
	_hoverRectangle->SetPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	_hoverUI = -1;

	//Set selected rectangle
	if (_selectedUI >= (int)_gameResponseVector.size())
		_selectedUI = -1;

	if (_selectedUI >= 0)
		_selectedRectangle->SetPoints(glm::vec4(32.0f, (float)(64+(_selectedUI*16)), 32.0f+textLengthf, (float)(64+(_selectedUI*16))+16.0f));
	else
		_selectedRectangle->SetPoints(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	//Draw text
	_guiTitleText->Draw(16, 0, "LAN Lobby", menu->GetCamera());

	_guiTitleText->Draw(16, 32, "Games Found:", menu->GetCamera());

	std::string broadcastStr = "Sending broadcast for games: (";
	broadcastStr.append(std::to_string(_broadcastsSent));
	broadcastStr.append(")");
	_guiText->Draw(columni, 32, broadcastStr, menu->GetCamera());

	//Selectable text
	_guiText->Draw(columni, 64, "Host Game", menu->GetCamera());
	if (mousePos.x > columnf && mousePos.x < columnf+textLengthf && mousePos.y > 64.0f && mousePos.y < 64.0f+16.0f)
	{
		_hoverRectangle->SetPoints(glm::vec4(columnf, 64.0f, columnf+textLengthf, 64.0f+16.0f));
		_hoverUI = LAN_HOVER_HOST;
	}

	_guiText->Draw(columni, 80, "Start Single Player Game", menu->GetCamera());
	if (mousePos.x > columnf && mousePos.x < columnf+textLengthf && mousePos.y > 80.0f && mousePos.y < 80.0f+16.0f)
	{
		_hoverRectangle->SetPoints(glm::vec4(columnf, 80.0f, columnf+textLengthf, 80.0f+16.0f));
		_hoverUI = LAN_HOVER_SOLO;
	}

	_guiText->Draw(columni, 128, "Join Game", menu->GetCamera());
	if (mousePos.x > columnf && mousePos.x < columnf+textLengthf && mousePos.y > 128.0f && mousePos.y < 128.0f+16.0f)
	{
		_hoverRectangle->SetPoints(glm::vec4(columnf, 128.0f, columnf+textLengthf, 128.0f+16.0f));
		_hoverUI = LAN_HOVER_JOIN;
	}

	for (unsigned int i = 0;i < _gameResponseVector.size();i ++)
	{
		_guiText->Draw(32, 64+(i*16), _gameResponseVector[i].GetGameName(), menu->GetCamera());
		if (mousePos.x > 32.0f && mousePos.x < 32.0f+textLengthf && mousePos.y > (float)(64+(i*16)) && mousePos.y < (float)(64+(i*16))+16.0f)
		{
			_hoverRectangle->SetPoints(glm::vec4(32.0f, (float)(64+(i*16)), 32.0f+textLengthf, (float)(64+(i*16))+16.0f));
			_hoverUI = LAN_HOVER_GAME;
		}
	}*/

	if (input->GetMouseInput(INPUT_MOUSE_RELEASE, INPUT_MOUSE_LEFTBUTTON))
	{
		switch (_hoverUI)
		{
		case LAN_HOVER_HOST: return 2; break;
		case LAN_HOVER_SOLO: return 1; break;
		case LAN_HOVER_JOIN: break;
		case LAN_HOVER_GAME: _selectedUI = (int)(mousePos.y-64.0f)/16; break;
		}
	}

	std::string gameString;
	if (menu->GetNetwork()->PollJoinBroadcast(&gameString) == 1)
		HandleNewGame(menu, gameString);

	if (menu->GetTime()-_ticker > 1000)
	{
		menu->GetNetwork()->SendJoinBroadcast();
		_broadcastsSent ++;

		_ticker = menu->GetTime();
	}

	CheckOldGames(menu);

	return 0;
}

int MMLANLobby::HandleNewGame(Menu* menu, std::string gameString)
{
	for (unsigned int i = 0;i < _gameResponseVector.size();i ++)
	{
		if (_gameResponseVector[i].GetGameName().compare(gameString) == 0)
		{
			_gameResponseVector[i].SetNewStartTime(menu->GetTime());
			return 1;
		}
	}

	_gameResponseVector.push_back(GameResponse());
	_gameResponseVector.back().Init(gameString, menu->GetTime(), 5000);

	return 0;
}

int MMLANLobby::CheckOldGames(Menu* menu)
{
	for (unsigned int i = 0;i < _gameResponseVector.size();i ++)
	{
		if (_gameResponseVector[i].GetEndTime() < menu->GetTime())
		{
			_gameResponseVector.erase(_gameResponseVector.begin()+i);

			if (_selectedUI == i)
				_selectedUI = -1;
			else if (_selectedUI > (int)i)
				_selectedUI --;
		}
	}

	return 0;
}
