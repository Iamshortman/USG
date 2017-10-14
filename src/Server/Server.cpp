#include "Server/Server.hpp"

#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/Entity/EntityGridSystem.hpp"
#include "Common/Entity/EntityCharacter.hpp"
#include "Common/Entity/EntityTempShip.hpp"

Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;

	networkManager = new ServerNetworkManager(32, 60000);

	rootWorlds.push_back(WorldManager::instance->createWorld(WORLDTYPE::SOLAR));

	//Spawn some stuff for testing
	EntityTempShip* ship = (EntityTempShip*)EntityManager::instance->createEntity(ENTITYTYPE::TEMPSHIP);
	ship->setTransform(Transform(vector3D(10.0, 5.0, 10.0)));
	ship->addToWorld(this->rootWorlds[0]);

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			EntityGridSystem* ship = (EntityGridSystem*)EntityManager::instance->createEntity(ENTITYTYPE::GRIDSYSTEM);
			ship->setTransform(Transform(vector3D(x * 2.0, 0.0, (y * 2.0) + 2)));
			ship->addToWorld(this->rootWorlds[0]);
		}
	}
	
}

Server::~Server()
{

}

void Server::update(double deltaTime)
{
	WorldManager::instance->update(deltaTime);


	for (ClientConnection* client : this->clients)
	{
		client->updateEntitiesInFocus();

		for (Entity* entity : client->entitiesInFocus)
		{
			if (client->entitiesLoaded.find(entity->entityId) == client->entitiesLoaded.end())
			{
				PacketSend packetCreate(PacketTypes::CreateEntity, HIGH_PRIORITY, RELIABLE);
				packetCreate.bitStream_out.Write(entity->getEntityType());
				packetCreate.bitStream_out.Write(entity->getWorld()->worldId);
				entity->writeNetworkPacket(&packetCreate.bitStream_out);
				this->networkManager->sendPacket(packetCreate, client->getUsername());

				if (entity->hasSubWorld())
				{
					PacketSend packetCreateW(PacketTypes::CreateWorld, HIGH_PRIORITY, RELIABLE);
					packetCreateW.bitStream_out.Write(entity->getSubWorld()->worldId);
					packetCreateW.bitStream_out.Write(entity->entityId);
					this->networkManager->sendPacket(packetCreateW, client->getUsername());
				}

				client->entitiesLoaded.insert(entity->entityId);
			}

			if(entity->getEntityType() == ENTITYTYPE::GRIDSYSTEM)
			{
				PacketSend packet(PacketTypes::UpdateEntity, LOW_PRIORITY, UNRELIABLE);
				entity->writeNetworkPacket(&packet.bitStream_out);
				this->networkManager->sendPacket(packet, client->getUsername());
			}
			else if (entity->getEntityType() == ENTITYTYPE::CHARACTOR && entity != client->getControllingEntity())
			{
				PacketSend packet(PacketTypes::UpdateEntity, LOW_PRIORITY, UNRELIABLE);
				entity->writeNetworkPacket(&packet.bitStream_out);
				this->networkManager->sendPacket(packet, client->getUsername());
			}

		}
	}
}

void Server::exitGame()
{
	shouldClose = true;
}
 
const bool Server::getShouldClose()
{
	return shouldClose;
}

void Server::addClient(string username)
{
	ClientConnection* client = new ClientConnection(username);
	this->clients.emplace(client);

	EntityCharacter* player = (EntityCharacter*)EntityManager::instance->createEntity(ENTITYTYPE::CHARACTOR);
	player->addToWorld(EntityManager::instance->getEntity(1)->getSubWorld());

	client->setControllingEntity(player);

	World* world = player->getWorld();
	client->worldsToLoad.push(world);

	while (world->hasParentWorld())
	{
		world = world->getParentWorld();
		client->worldsToLoad.push(world);
	}

	/*client->entitiesLoaded.insert(player->entityId);

	PacketSend packetCreateW(PacketTypes::CreateWorld, HIGH_PRIORITY, RELIABLE);
	packetCreateW.bitStream_out.Write(player->getWorld()->worldId);
	packetCreateW.bitStream_out.Write(0);
	this->networkManager->sendPacket(packetCreateW, client->getUsername());

	PacketSend packetCreate(PacketTypes::CreateEntity, HIGH_PRIORITY, RELIABLE);
	packetCreate.bitStream_out.Write(ENTITYTYPE::PLAYER_THIS);
	packetCreate.bitStream_out.Write(player->getWorld()->worldId);
	player->writeNetworkPacket(&packetCreate.bitStream_out);
	this->networkManager->sendPacket(packetCreate, username);

	PacketSend packet(PacketTypes::ClientBindEntity, HIGH_PRIORITY, RELIABLE);
	packet.bitStream_out.Write(player->entityId);
	this->networkManager->sendPacket(packet, username);*/
}

void Server::removeClient(string username)
{
	for (ClientConnection* client : this->clients)
	{
		if (client->getUsername() == username)
		{
			this->clients.erase(client);
			delete client;
			break;
		}
	}
}