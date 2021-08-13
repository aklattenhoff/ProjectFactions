#include "MMLANHost.h"

MMLANHost::MMLANHost()
{
	_guiTitleText = NULL;
	_guiText = NULL;
	_hoverRectangle = NULL;
	_selectedRectangle = NULL;
}

MMLANHost::~MMLANHost()
{
	delete _guiTitleText;
	delete _guiText;
	delete _hoverRectangle;
	delete _selectedRectangle;
}

int MMLANHost::Init()
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

	return 0;
}

int MMLANHost::Render(Menu* menu)
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

	//Draw text
	_guiTitleText->Draw(16, 0, "LAN Host", menu->GetCamera());

	_guiTitleText->Draw(16, 32, "Players in game:", menu->GetCamera());

	std::string broadcastStr = "In communication with: (";
	broadcastStr.append(std::to_string(_addressVector.size()));
	broadcastStr.append(") clients");
	_guiText->Draw(columni, 32, broadcastStr, menu->GetCamera());

	//List
	for (unsigned int i = 0;i < _addressVector.size();i ++)
	{
		_guiText->Draw(columni, 64+(i*16), _addressVector[i].address+" : "+std::to_string(_addressVector[i].port), menu->GetCamera());
	}*/

	if (input->GetMouseInput(INPUT_MOUSE_RELEASE, INPUT_MOUSE_LEFTBUTTON))
		return 2;

	Network::Packet packet;
	if (menu->GetNetwork()->PollHostBroadcast(&packet) == 1)
		HandleBroadcastPacket(menu, packet);

	CheckOldClients(menu);

	return 0;
}

int MMLANHost::HandleBroadcastPacket(Menu* menu, Network::Packet packet)
{
	/*if (packet.message[0] != 'J')
		return 0;

	Network::Packet sendPacket;
	sendPacket.endpoint = packet.endpoint;
	sendPacket.message = "Some Game";
	menu->GetNetwork()->SendBroadcastResponse(sendPacket);

	std::string address = packet.endpoint.address().to_string();
	for (unsigned int i = 0;i < _addressVector.size();i ++)
	{
		if (_addressVector[i].address.compare(address) == 0)
		{
			_addressVector[i]._startTime = menu->GetTime();
			return 0;
		}
	}

	ClientAddress clientAddress;

	clientAddress.address = packet.endpoint.address().to_string();
	clientAddress.port = packet.endpoint.port();
	clientAddress._startTime = menu->GetTime();
	clientAddress._lifeTime = 5000;

	_addressVector.push_back(clientAddress);*/

	return 0;
}

int MMLANHost::CheckOldClients(Menu* menu)
{
	for (unsigned int i = 0;i < _addressVector.size();i ++)
	{
		if (_addressVector[i]._startTime + _addressVector[i]._lifeTime < menu->GetTime())
		{
			_addressVector.erase(_addressVector.begin()+i);
		}
	}

	return 0;
}
