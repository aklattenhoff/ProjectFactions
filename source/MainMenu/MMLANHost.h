#ifndef _MM_LAN_HOST_H
#define _MM_LAN_HOST_H

#include <string>
#include <vector>

//#include "Graphics/Camera.h"
#include "Game/GUI/GUIText.h"
#include "Game/GUI/GUIRectangle.h"

#include "MainMenu/Menu.h"

#include "Network/Network.h"
#include "Network/GameResponse.h"

class MMLANHost
{
public:
	MMLANHost();
	~MMLANHost();

	int Init();
	int Render(Menu* menu);
private:
	int HandleBroadcastPacket(Menu* menu, Network::Packet packet);
	int CheckOldClients(Menu* menu);
private:
	struct ClientAddress
	{
		std::string address;
		unsigned short port;
		unsigned int _startTime;
		unsigned int _lifeTime;
	};
private:
	GUIText* _guiTitleText;
	GUIText* _guiText;
	GUIRectangle* _hoverRectangle;
	GUIRectangle* _selectedRectangle;
	int _hoverUI;

	std::vector<ClientAddress> _addressVector;
};
#endif
