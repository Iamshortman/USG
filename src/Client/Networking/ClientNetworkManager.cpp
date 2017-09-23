#include "Client/Networking/ClientNetworkManager.hpp"

ClientNetworkManager::ClientNetworkManager(string server_ip, int server_port)
{
	this->server_ip = server_ip;

	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);

	printf("Connecting to ServerIP %s\n", server_ip.c_str());
	peer->Connect(server_ip.c_str(), server_port, 0, 0);

	network_listener = std::thread(&ClientNetworkManager::listen, this);
}

ClientNetworkManager::~ClientNetworkManager()
{
	this->shouldClose = true;
	network_listener.join();
}

void ClientNetworkManager::listen()
{
	printf("Starting NetworkLoop\n");

	RakNet::Packet *packet;

	while (shouldClose == false)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				printf("Our connection request has been accepted.\n");

				PacketSend packetSend(PacketTypes::UserConnect, HIGH_PRIORITY, RELIABLE_ORDERED);
				packetSend.write_String("Player1");
				this->sendPacket(packetSend);

				PacketSend packetSend1(PacketTypes::UserRequest, HIGH_PRIORITY, RELIABLE_ORDERED);
				packetSend1.write_String("SpawnData");
				this->sendPacket(packetSend1);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost.\n");
				break;
			}

			if (packet->data[0] >= PacketTypes::UserConnect)
			{

			}

		}
	}

	peer->Shutdown(100);
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void ClientNetworkManager::sendPacket(PacketSend &packet)
{
	peer->Send(&packet.bitStream_out, packet.packet_priority, packet.packet_reliability, 0, peer->GetSystemAddressFromIndex(0), false);
}