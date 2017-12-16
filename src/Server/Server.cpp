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
	ship->setSubWorld(WorldManager::instance->createWorld(WORLDTYPE::BASE));
	ship->setTransform(Transform(vector3D(10.0, 5.0, 10.0)));
	ship->addToWorld(this->rootWorlds[0]);

	/*Entity* entity = (EntityGridSystem*)EntityManager::instance->createEntity(ENTITYTYPE::GRIDSYSTEM);
	entity->addToWorld(ship->getSubWorld());*/

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			EntityGridSystem* cube = (EntityGridSystem*)EntityManager::instance->createEntity(ENTITYTYPE::GRIDSYSTEM);
			cube->setTransform(Transform(vector3D(x * 2.0, 0.0, (y * 2.0) + 2)));
			cube->addToWorld(this->rootWorlds[0]);
		}
	}
	
}

Server::~Server()
{

}

void Server::update(double deltaTime)
{
	this->networkManager->processPackets();
	
	WorldManager::instance->update(deltaTime);
	EntityManager::instance->update();

	for (ClientConnection* client : this->clients)
	{
		client->updateEntitiesInFocus();

		while (!client->worldsToLoad.empty())
		{
			WorldId worldId = client->worldsToLoad.front();

			if (client->worldsInFocus.find(worldId) == client->worldsInFocus.end())
			{
				World* world = WorldManager::instance->getWorld(worldId);
				PacketSend packetCreate(PacketTypes::CreateWorld, HIGH_PRIORITY, RELIABLE);
				packetCreate.bitStream_out.Write(world->getWorldType());
				packetCreate.bitStream_out.Write(world->worldId);

				if (world->getParent() != nullptr)
				{
					packetCreate.bitStream_out.Write(world->getParent()->entityId);
				}
				else
				{
					packetCreate.bitStream_out.Write((EntityId)0);
				}

				this->networkManager->sendPacket(packetCreate, client);

				client->worldsInFocus.insert(worldId);
			}

			client->worldsToLoad.pop();
		}

		while (!client->entitiesToLoad.empty())
		{
			EntityId entityId = client->entitiesToLoad.front();

			if (client->entitiesInFocus.find(entityId) == client->entitiesInFocus.end())
			{
				Entity* entity = EntityManager::instance->getEntity(entityId);
				PacketSend packetCreate(PacketTypes::CreateEntity, HIGH_PRIORITY, RELIABLE);
				packetCreate.bitStream_out.Write(entity->getEntityType());
				packetCreate.bitStream_out.Write(entity->entityId);
				packetCreate.bitStream_out.Write(entity->getWorld()->worldId);
				if (entity->hasSubWorld())
				{
					packetCreate.bitStream_out.Write(entity->getSubWorld()->worldId);
					client->worldsToLoad.push(entity->getSubWorld()->worldId);
				}
				else
				{
					packetCreate.bitStream_out.Write((WorldId)0);
				}

				entity->writeNetworkPacket(&packetCreate.bitStream_out);
				this->networkManager->sendPacket(packetCreate, client);

				client->entitiesInFocus.insert(entityId);
			}

			client->entitiesToLoad.pop();
		}

		for (EntityId entityId : client->entitiesInFocus)
		{
			Entity* entity = EntityManager::instance->getEntity(entityId);
			if (entity == nullptr)
			{
				client->entitiesToUnload.push(entityId);
				continue;
			}

			if (entity != client->getControllingEntity())
			{
				PacketSend packet(PacketTypes::UpdateEntity, LOW_PRIORITY, UNRELIABLE);
				packet.bitStream_out.Write(entity->entityId);
				entity->writeNetworkPacket(&packet.bitStream_out);
				this->networkManager->sendPacket(packet, client);
			}
		}

		while (!client->worldsToUnload.empty())
		{
			WorldId worldId = client->worldsToUnload.front();
			client->worldsToUnload.pop();

			if (client->entitiesInFocus.find(worldId) != client->entitiesInFocus.end())
			{
				client->entitiesInFocus.erase(worldId);

				PacketSend packet(PacketTypes::DeleteWorld, LOW_PRIORITY, RELIABLE);
				packet.bitStream_out.Write(worldId);
				this->networkManager->sendPacket(packet, client);
			}
		}

		while (!client->entitiesToUnload.empty())
		{
			EntityId entityId = client->entitiesToUnload.front();
			client->entitiesToUnload.pop();

			if (client->entitiesInFocus.find(entityId) != client->entitiesInFocus.end())
			{
				client->entitiesInFocus.erase(entityId);

				PacketSend packet(PacketTypes::DeleteEntity, LOW_PRIORITY, RELIABLE);
				packet.bitStream_out.Write(entityId);
				this->networkManager->sendPacket(packet, client);
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

void addWorld(ClientConnection* client, World* world)
{
	client->worldsToLoad.push(world->worldId);

	auto entities = world->getEntitiesInWorld();
	for (auto entity = entities->begin(); entity != entities->end(); entity++)
	{
		client->entitiesToLoad.push((*entity)->entityId);
	}
}


void Server::addClient(RakNet::SystemAddress address)
{
	ClientConnection* client = new ClientConnection(address);
	this->clients.emplace(client);

	EntityCharacter* player = (EntityCharacter*)EntityManager::instance->createEntity(ENTITYTYPE::CHARACTOR);
	player->addToWorld(WorldManager::instance->getWorld(1));

	client->setControllingEntity(player);

	World* world = player->getWorld();

	client->entitiesInFocus.insert(player->entityId);
	client->worldsInFocus.insert(player->getWorld()->worldId);
	PacketSend packetCreateW(PacketTypes::CreateWorld, HIGH_PRIORITY, RELIABLE);
	packetCreateW.bitStream_out.Write(player->getWorld()->getWorldType());
	packetCreateW.bitStream_out.Write(player->getWorld()->worldId);
	packetCreateW.bitStream_out.Write((EntityId)0);
	this->networkManager->sendPacket(packetCreateW, client);
	PacketSend packetCreate(PacketTypes::CreateEntity, HIGH_PRIORITY, RELIABLE);
	packetCreate.bitStream_out.Write(ENTITYTYPE::PLAYER_THIS);
	packetCreate.bitStream_out.Write(player->entityId);
	packetCreate.bitStream_out.Write(player->getWorld()->worldId);
	packetCreate.bitStream_out.Write((WorldId)0);
	player->writeNetworkPacket(&packetCreate.bitStream_out);
	this->networkManager->sendPacket(packetCreate, client);
	PacketSend packet(PacketTypes::ClientBindEntity, HIGH_PRIORITY, RELIABLE);
	packet.bitStream_out.Write(player->entityId);
	this->networkManager->sendPacket(packet, client);

	addWorld(client, world);

	auto worlds = world->getSubWorlds();
	for (auto subWorld = worlds->begin(); subWorld != worlds->end(); subWorld++)
	{
		addWorld(client, *subWorld);
	}

	while (world->hasParentWorld())
	{
		world = world->getParentWorld();

		addWorld(client, world);

		if (world->getParent() != nullptr)
		{
			client->entitiesToLoad.push(world->getParent()->entityId);
		}
	}

}

void Server::removeClient(RakNet::SystemAddress address)
{
	for (ClientConnection* client : this->clients)
	{
		if (client->getAddress() == address)
		{
			client->closeConnection();
			this->clients.erase(client);
			delete client;
			return;
		}
	}
}

