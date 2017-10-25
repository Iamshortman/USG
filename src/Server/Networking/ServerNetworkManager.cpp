#include "Server/Networking/ServerNetworkManager.hpp"
#include "Common/World/World.hpp"

#include "Server/Server.hpp"
#include "Common/Entity/EntityManager.hpp"
#include "Common/Entity/EntityCharacter.hpp"

ServerNetworkManager::ServerNetworkManager(unsigned int maxClients, unsigned int serverPort)
{
	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(serverPort, 0);;
	peer->Startup(maxClients, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(maxClients);
	network_listener = std::thread(&ServerNetworkManager::listen, this);
}

ServerNetworkManager::~ServerNetworkManager()
{
	this->shouldClose = true;
	network_listener.join();
}

void ServerNetworkManager::listen()
{
	printf("Starting Network Loop\n");

	RakNet::Packet *packet;

	while (shouldClose == false)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			if (packet->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS)
			{
				printf("The server is full.\n");
				continue;
			}

			PacketData data;
			data.systemAddress = packet->systemAddress;
			data.length = packet->length;
			data.data = (unsigned char*)malloc(packet->length);
			memcpy_s(data.data, data.length, packet->data, packet->length);

			this->queueMutex.lock();
			this->packetsToProcess.push(data);
			this->queueMutex.unlock();

			/*(switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			case ID_NEW_INCOMING_CONNECTION:
				
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
				Server::instance->removeClient(packet->systemAddress);
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}*/
		}
	}

	peer->Shutdown(100);
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void ServerNetworkManager::sendPacket(PacketSend &packet, ClientConnection* client)
{
	peer->Send(&packet.bitStream_out, packet.packet_priority, packet.packet_reliability, 0, client->getAddress(), false);
}

void ServerNetworkManager::sendPacketToAll(PacketSend &packet)
{
	peer->Send(&packet.bitStream_out, packet.packet_priority, packet.packet_reliability, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ServerNetworkManager::update()
{

}

void ServerNetworkManager::processPackets()
{
	this->queueMutex.lock();
	while (!this->packetsToProcess.empty())
	{
		PacketData packet = this->packetsToProcess.front();
		
		if (packet.data[0] == PacketTypes::UpdateClientEntity)
		{
			BitStream bsIn(packet.data, packet.length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			EntityId id;
			bsIn.Read(id);
			EntityCharacter* entity = (EntityCharacter*)EntityManager::instance->getEntity(id);
			if (entity != nullptr)
			{
				entity->readNetworkPacket(&bsIn);
			}
		}
		else if (packet.data[0] == ID_NEW_INCOMING_CONNECTION)
		{
			//User connect
			printf("Client has connected at address: %s\n", packet.systemAddress.ToString());
			Server::instance->addClient(packet.systemAddress);
		}
		else if (packet.data[0] == ID_DISCONNECTION_NOTIFICATION)
		{
			//Clean disconnect
			printf("Client has cleanly disconnected at address: %s\n", packet.systemAddress.ToString());
			Server::instance->removeClient(packet.systemAddress);
		}
		else if (packet.data[0] == ID_CONNECTION_LOST)
		{
			//Unclean disconnect
			printf("Client has lost connection at address: %s\n", packet.systemAddress.ToString());
			Server::instance->removeClient(packet.systemAddress);
		}

		free(packet.data);
		this->packetsToProcess.pop();
	}
	this->queueMutex.unlock();
}