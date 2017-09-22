#ifndef SERVERNETWORKMANAGER_HPP
#define SERVERNETWORKMANAGER_HPP

#include <thread>
#include <unordered_map>

#include "Common/Types.hpp"
#include "Common/Networking/Packet.hpp"
#include "Server/Networking/ClientConnection.hpp"


#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

class ServerNetworkManager
{
public:
	ServerNetworkManager(unsigned int maxClients, unsigned int serverPort);
	~ServerNetworkManager();

	void listen();

	void sendPacket(PacketSend &packet);

private:
	RakNet::RakPeerInterface *peer;
	std::thread network_listener;
	bool shouldClose = false;

	std::unordered_map<string, RakNet::SystemAddress> usernameAddressMap;
	//std::unordered_map<RakNet::SystemAddress, ClientConnection*> connectedClients;
};

#endif //SERVERNETWORKMANAGER_HPP