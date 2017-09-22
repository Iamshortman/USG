#include "Server/Networking/ServerNetworkManager.hpp"

ServerNetworkManager::ServerNetworkManager(unsigned int maxClients, unsigned int serverPort)
{
	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(serverPort, 0);;
	peer->Startup(maxClients, &sd, 1);

	printf("Starting the server.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(maxClients);


	/*peer->Startup(1, &sd, 1);

	printf("Connecting to ServerIP %s\n", server_ip.c_str());
	peer->Connect(server_ip.c_str(), SERVER_PORT, 0, 0);*/

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
				case ID_NEW_INCOMING_CONNECTION:
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
				case ID_CONNECTION_LOST:
					for (auto it : this->usernameAddressMap)
					{
						if (it.second == packet->systemAddress)
						{
							printf("%s at address %s has disconnected\n", it.first.c_str(), it.second.ToString());
							this->usernameAddressMap.erase(it.first);
							break;
						}
					}
					break;
				case UserConnect:
				{
					RakNet::RakString rs;
					RakNet::BitStream bsIn(packet->data, packet->length, false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read(rs);
					string username = string(rs.C_String());

					if (this->usernameAddressMap.find(username) == this->usernameAddressMap.end())
					{
						printf("%s at address %s has connected\n", username.c_str(), packet->systemAddress.ToString());
						this->usernameAddressMap[username] = packet->systemAddress;
					}
					else
					{
						printf("Error: Client alread connected with this username %s\n", username.c_str());
					}

				}
				break;

				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
			}
	}

	peer->Shutdown(100);
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void ServerNetworkManager::sendPacket(PacketSend &packet)
{
	peer->Send(&packet.bitStream_out, packet.packet_priority, packet.packet_reliability, 0, peer->GetSystemAddressFromIndex(0), false);
}