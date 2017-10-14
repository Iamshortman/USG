#include "Client/Networking/ClientNetworkManager.hpp"
#include "Client/GameState/GameState.hpp"

#include "Common/Entity/EntityManager.hpp"
#include "Common/World/WorldManager.hpp"
#include "Client/Entity/EntityPlayerClient.hpp"

ClientNetworkManager::ClientNetworkManager(string server_ip, int server_port, GameState_Multiplayer* game)
{
	this->game = game;

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
	printf("Closing Connection\n");
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
			if (packet->data[0] > ID_USER_PACKET_ENUM)
			{
				PacketData data;
				data.length = packet->length;
				data.data = (unsigned char*)malloc(packet->length);
				memcpy_s(data.data, data.length, packet->data, packet->length);

				this->queueMutex.lock();
				this->packetsToProcess.push(data);
				this->queueMutex.unlock();
			}
			
			switch (packet->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				isConnected = true;
				printf("Our connection request has been accepted.\n");

				PacketSend packetSend(PacketTypes::UserConnect, HIGH_PRIORITY, RELIABLE_ORDERED);
				packetSend.bitStream_out.Write("Player1");
				this->sendPacket(packetSend);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				isConnected = false;
				printf("We have been disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				isConnected = false;
				printf("Connection lost.\n");
				break;
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

void ClientNetworkManager::processPackets()
{
	this->queueMutex.lock();
	while (!this->packetsToProcess.empty())
	{
		PacketData packet = this->packetsToProcess.front();

		
		if (packet.data[0] == PacketTypes::UpdateEntity)
		{
			BitStream bsIn(packet.data, packet.length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			EntityId id;
			bsIn.Read(id);
			Entity* entity = EntityManager::instance->getEntity(id);
			if (entity != nullptr)
			{
				entity->readNetworkPacket(&bsIn);
			}
		}
		else if (packet.data[0] == PacketTypes::CreateEntity)
		{
			BitStream bsIn(packet.data, packet.length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			EntityManager::instance->createEntityFromNetwork(&bsIn);
		}
		else if (packet.data[0] == PacketTypes::ClientBindEntity)
		{
			BitStream bsIn(packet.data, packet.length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			EntityId entityId;
			bsIn.Read(entityId);
			Entity* entity = EntityManager::instance->getEntity(entityId);

			if (entity != nullptr)
			{
				if (entity->getEntityType() == ENTITYTYPE::PLAYER_THIS)
				{
					printf("Client Bind\n");
					EntityPlayerClient* player = (EntityPlayerClient*)entity;
					this->game->playerInterface.bindCharacter(player);
				}
			}
		}
		else if (packet.data[0] == PacketTypes::CreateWorld)
		{
			BitStream bsIn(packet.data, packet.length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			WorldId id;
			vector3D gravity;
			vector3F ambientLight;
			EntityId parentId;

			bsIn.Read(id);
			//bsIn.Read(gravity);
			//bsIn.Read(ambientLight);
			bsIn.Read(parentId);

			World* world = WorldManager::instance->createWorld(WORLDTYPE::BASE, id);
			//world->setGravity(gravity);
			//world->ambientLight = ambientLight;

			if (parentId != 0)
			{
				Entity* entity = EntityManager::instance->getEntity(parentId);
				if (entity != nullptr)
				{
					entity->setSubWorld(world);
				}
			}

		}

		free(packet.data);
		this->packetsToProcess.pop();
	}
	this->queueMutex.unlock();
}