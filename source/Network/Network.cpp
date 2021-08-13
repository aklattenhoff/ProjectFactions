#include "Network.h"

#include <string>

Network::Network()
{
	//_joinSocket = NULL;
	//_hostSocket = NULL;

	_buffer = new char[256];
	memset(_buffer, 0, 256);
}

Network::~Network()
{
	//if (_joinSocket)
		//DestroyJoinSocket();
	//delete _joinSocket;
	//if (_hostSocket)
		//DestroyHostSocket();
	//delete _hostSocket;
	//delete[] _buffer;
}

/*void handleTest(const asio::error_code& error, std::size_t bytesTransferred)
{
	printf("Received %i bytes with error:\n   %s\n", bytesTransferred, error.message().c_str());
}*/

int Network::Init()
{
	//_broadcastEndpoint = asio::ip::udp::endpoint(asio::ip::address_v4::broadcast(), 8991);

	return 0;
}

int Network::CreateJoinSocket()
{
	/*_joinSocket = new asio::ip::udp::socket(_joinService, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
	_joinService.reset();
	_joinService.poll();*/

	return 0;
}

int Network::DestroyJoinSocket()
{
	/*_joinService.stop();
	_joinSocket->shutdown(asio::ip::udp::socket::shutdown_both);
	_joinSocket->close();
	//_joinService.poll();
	delete _joinSocket;
	_joinSocket = NULL;
	//_joinService.reset();*/

	return 0;
}

int Network::StartJoinBroadcast()
{
	/*if (!_joinSocket)
		CreateJoinSocket();

	memset(_buffer, 0, 256);
	_joinSocket->async_receive_from(asio::buffer(_buffer, 256), _remoteEndpoint, handleTest);*/

	return 0;
}

int Network::SendJoinBroadcast()
{
	/*if (!_joinSocket)
		CreateJoinSocket();

	_joinSocket->set_option(asio::socket_base::broadcast(true));

	std::string sendBuffer = "Join Broadcast";
	_joinSocket->send_to(asio::buffer(sendBuffer), _broadcastEndpoint);*/

	return 0;
}

int Network::PollJoinBroadcast(std::string* outString)
{
	/*if (!_joinSocket)
		CreateJoinSocket();

	int returnValue = 0;

	if (_joinService.poll_one() > 0)
	{
		printf("Handled response about join\n");
		(*outString) = _buffer;
		memset(_buffer, 0, 256);
		_joinSocket->async_receive_from(asio::buffer(_buffer, 256), _remoteEndpoint, handleTest);
		returnValue =  1;
	}*/

	//return returnValue;
	return 0;
}

int Network::CreateHostSocket()
{
	/*_hostSocket = new asio::ip::udp::socket(_hostService, asio::ip::udp::endpoint(asio::ip::udp::v4(), 8991));
	_hostService.reset();
	_hostService.poll();*/

	return 0;
}

int Network::DestroyHostSocket()
{
	/*_hostService.stop();
	_hostSocket->shutdown(asio::ip::udp::socket::shutdown_both);
	_hostSocket->close();
	//_hostService.poll();
	delete _hostSocket;
	_hostSocket = NULL;
	//_hostService.reset();*/

	return 0;
}

int Network::StartHostBroadcast()
{
	/*if (!_hostSocket)
		CreateHostSocket();

	memset(_buffer, 0, 256);
	_hostSocket->async_receive_from(asio::buffer(_buffer, 256), _remoteEndpoint, handleTest);*/

	return 0;
}

int Network::PollHostBroadcast(Network::Packet* outPacket)
{
	/*if (!_hostSocket)
		CreateHostSocket();

	int returnValue = 0;

	if (_hostService.poll_one() > 0)
	{
		printf("Handled broadcast: %s:%i\n   %s\n", _remoteEndpoint.address().to_string().c_str(), _remoteEndpoint.port(), _buffer);
		outPacket->message = _buffer;
		outPacket->endpoint = _remoteEndpoint;
		memset(_buffer, 0, 256);
		_hostSocket->async_receive_from(asio::buffer(_buffer, 256), _remoteEndpoint, handleTest);
		returnValue =  1;
	}*/

	//return returnValue;
	return 0;
}

int Network::SendBroadcastResponse(Network::Packet packet)
{
	/*if (!_hostSocket)
		CreateHostSocket();

	printf("sending message: %s:%i\n", packet.endpoint.address().to_string().c_str(), packet.endpoint.port());
	_hostSocket->send_to(asio::buffer(packet.message), packet.endpoint);*/

	return 0;
}
