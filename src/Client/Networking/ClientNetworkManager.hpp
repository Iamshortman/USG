#ifndef CLIENTNETWORKMANAGER_HPP
#define CLIENTNETWORKMANAGER_HPP

#include <thread>
#include <mutex>
#include <queue>

#include "Common/Types.hpp"
#include "Common/Networking/Packet.hpp"

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"

struct PacketData
{
	unsigned char* data;
	unsigned int length;
};

//Prototype Class
class GameState_Multiplayer;

class ClientNetworkManager
{
public:
	ClientNetworkManager(string server_ip, int server_port, GameState_Multiplayer* game);
	~ClientNetworkManager();

	void listen();

	void sendPacket(PacketSend &packet);

	void processPackets();

	bool isConnected = false;
private:
	GameState_Multiplayer* game;

	string server_ip;
	RakNet::RakPeerInterface *peer;
	std::thread network_listener;
	bool shouldClose = false;

	std::mutex queueMutex;
	std::queue<PacketData> packetsToProcess;
};

#endif //CLIENTNETWORKMANAGER_HPP