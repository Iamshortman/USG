#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/Entity/EntityGridSystem.hpp"
#include "Client/Entity/EntityPlayerClient.hpp"
#include "Common/Entity/EntityTempShip.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->mainWorld = WorldManager::instance->createWorld(WORLDTYPE::SOLAR);
	EntityPlayerClient* player = new EntityPlayerClient(EntityManager::instance->getNextId());
	EntityManager::instance->entities[player->entityId] = player;

	this->playerInterface.bindCharacter(player);

	player->addToWorld(this->mainWorld);

	//Spawn some stuff for testing
	for (int x = 0; x < 10; x++)
	{
		for (int y = 1; y < 11; y++)
		{
			EntityGridSystem* ship = (EntityGridSystem*)EntityManager::instance->createEntity(ENTITYTYPE::GRIDSYSTEM);
			ship->setTransform(Transform(vector3D(x * 2.0, 0.0, y * 2.0)));

			ship->addToWorld(this->mainWorld);
		}
	}

	EntityTempShip* ship = (EntityTempShip*)EntityManager::instance->createEntity(ENTITYTYPE::TEMPSHIP);
	ship->setTransform(Transform(vector3D(10.0, 5.0, 10.0)));
	ship->addToWorld(this->mainWorld);
	ship->setSubWorld(WorldManager::instance->createWorld(BASE));

}

GameState_Singleplayer::~GameState_Singleplayer()
{
	if (this->mainWorld != nullptr)
	{
		delete this->mainWorld;
	}
}

void GameState_Singleplayer::update(Client* client, double deltaTime)
{
	this->playerInterface.updatePlayerInput();
	WorldManager::instance->update(deltaTime);

	Entity* player = this->playerInterface.getBoundCharacter();
	Transform transform = this->playerInterface.getCameraTransform();
	Camera cam;
	cam.setCameraTransform(transform);
	client->window->clearBuffer();
	if (player != nullptr)
	{
		client->renderingManager->RenderWorld(player->getWorld(), &cam);
	}
	client->window->updateBuffer();
}