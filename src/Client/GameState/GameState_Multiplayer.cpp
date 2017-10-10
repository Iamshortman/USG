#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"
#include "Common/Entity/EntityGridSystem.hpp"
#include "Client/Entity/EntityPlayerClient.hpp"

GameState_Multiplayer::GameState_Multiplayer()
{
	this->networkManager = new ClientNetworkManager("127.0.0.1", 60000, this);

	this->mainWorld = WorldManager::instance->createNewWorldSolarSystem();
	/*EntityPlayerCharacter* player = new EntityPlayerCharacter(EntityManager::instance->getNextId());
	EntityManager::instance->entities[player->entityId] = player;

	this->playerInterface.bindCharacter(player);

	player->addToWorld(this->mainWorld);

	//Spawn some stuff for testing
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			EntityGridSystem* ship = (EntityGridSystem*)EntityManager::instance->createEntity(ENTITYTYPE::GRIDSYSTEM);
			ship->setTransform(Transform(vector3D(x * 2.0, 0.0, (y * 2.0) + 2)));
			ship->addToWorld(this->mainWorld);
		}
	}*/
}

GameState_Multiplayer::~GameState_Multiplayer()
{
	if (this->mainWorld != nullptr)
	{
		delete this->mainWorld;
	}
	delete this->networkManager;
}

void GameState_Multiplayer::update(Client* client, double deltaTime)
{
	//Update from network
	if (this->networkManager->isConnected == false)
	{
		//Draw Black Screen
		client->window->clearBuffer();
		client->window->updateBuffer();
		return;
	}

	this->networkManager->processPackets();

	this->playerInterface.updatePlayerInput();
	WorldManager::instance->update(deltaTime);

	Entity* player = this->playerInterface.getBoundCharacter();
	if (player != nullptr)
	{
		PacketSend send(PacketTypes::UpdateClientEntity, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED);
		player->writeNetworkPacket(&send.bitStream_out);
		this->networkManager->sendPacket(send);
	}


	Transform transform = this->playerInterface.getCameraTransform();
	Camera cam;
	cam.setCameraTransform(transform);
	client->window->clearBuffer();
	client->renderingManager->RenderWorld(this->mainWorld, &cam);
	client->window->updateBuffer();
}