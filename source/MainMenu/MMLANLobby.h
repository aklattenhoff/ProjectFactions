#ifndef _MM_LAN_LOBBY_H
#define _MM_LAN_LOBBY_H

#include <string>
#include <vector>

//#include "Graphics/Camera.h"
#include "Game/GUI/GUIText.h"
#include "Game/GUI/GUIRectangle.h"

#include "MainMenu/Menu.h"

#include "Network/GameResponse.h"

class MMLANLobby
{
public:
	MMLANLobby();
	~MMLANLobby();

	int Init();
	int Render(Menu* menu);
private:
	int HandleNewGame(Menu* menu, std::string gameString);
	int CheckOldGames(Menu* menu);
private:
	static const int LAN_HOVER_HOST = 0;
	static const int LAN_HOVER_SOLO = 1;
	static const int LAN_HOVER_JOIN = 2;
	static const int LAN_HOVER_GAME = 3;
private:
	GUIText* _guiTitleText;
	GUIText* _guiText;
	GUIRectangle* _hoverRectangle;
	GUIRectangle* _selectedRectangle;
	int _hoverUI;
	int _selectedUI;

	unsigned int _ticker;
	int _broadcastsSent;

	std::vector<GameResponse> _gameResponseVector;
};
#endif
