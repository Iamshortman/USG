#ifndef SERVERNETWORKMANAGER_HPP
#define SERVERNETWORKMANAGER_HPP

#include <thread>
#include <unordered_map>
#include <mutex>
#include <queue>

#include "Common/Types.hpp"
#include "Common/Networking/Packet.hpp"
#include "Server/Networking/ClientConnection.hpp"

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

struct PacketData
{
	RakNet::SystemAddress systemAddress;
	unsigned char* data;
	unsigned int length;
};

class ServerNetworkManager
{
public:
	ServerNetworkManager(unsigned int maxClients, unsigned int serverPort);
	~ServerNetworkManager();

	void listen(); //Thread

	void sendPacket(PacketSend &packet, ClientConnection* client);
	void sendPacketToAll(PacketSend &packet);

	void update();
	
	void processPackets();

private:
	RakNet::RakPeerInterface *peer;
	std::thread network_listener;
	bool shouldClose = false;

	//std::unordered_map<string, RakNet::SystemAddress> usernameAddressMap;
	//std::set<ClientConnection*> clients;
	//std::unordered_map<string, ClientConnection*> usernameMap;
	//std::unordered_map<RakNet::SystemAddress, ClientConnection*> Address;

	std::mutex queueMutex;
	std::queue<PacketData> packetsToProcess;
};

#endif //SERVERNETWORKMANAGER_HPP