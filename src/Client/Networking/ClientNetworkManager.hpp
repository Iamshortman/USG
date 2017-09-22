#ifndef CLIENTNETWORKMANAGER_HPP
#define CLIENTNETWORKMANAGER_HPP

#include <thread>


#include "Common/Types.hpp"
#include "Common/Networking/Packet.hpp"

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

class ClientNetworkManager
{
public:
	ClientNetworkManager(string server_ip, int server_port);
	~ClientNetworkManager();

	void listen();

	void sendPacket(PacketSend &packet);

private:
	string server_ip;
	RakNet::RakPeerInterface *peer;
	std::thread network_listener;
	bool shouldClose = false;
};

#endif //CLIENTNETWORKMANAGER_HPP