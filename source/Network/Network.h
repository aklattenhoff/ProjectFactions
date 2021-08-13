#ifndef _NETWORK_H
#define _NETWORK_H

#include <string>

class Network
{
public:
	struct Packet
	{
		std::string message;
		//asio::ip::udp::endpoint endpoint;
	};
public:
	Network();
	~Network();

	int Init();

	//Join
	int CreateJoinSocket();
	int DestroyJoinSocket();
	int StartJoinBroadcast();
	int SendJoinBroadcast();
	int PollJoinBroadcast(std::string* outString);

	//Host
	int CreateHostSocket();
	int DestroyHostSocket();
	int StartHostBroadcast();
	int PollHostBroadcast(Packet* outPacket);
	int SendBroadcastResponse(Packet packet);
private:
	/*asio::io_service _joinService;
	asio::io_service _hostService;
	asio::ip::udp::socket* _joinSocket;
	asio::ip::udp::socket* _hostSocket;

	asio::ip::udp::endpoint _broadcastEndpoint;
	asio::ip::udp::endpoint _remoteEndpoint;*/

	char* _buffer;
};

#endif
