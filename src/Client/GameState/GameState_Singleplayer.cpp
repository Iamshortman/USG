#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"

#include "Common/Entity/EntityGridSystem.hpp"
#include "Client/Entity/EntityPlayerClient.hpp"
#include "Common/Entity/EntityTempShip.hpp"
#include "Common/Entity/EntityNode.hpp"

GameState_Singleplayer::GameState_Singleplayer()
{
	this->mainWorld = WorldManager::instance->createWorld(WORLDTYPE::SOLAR);

	EntityTempShip* ship = (EntityTempShip*)EntityManager::instance->createEntity(ENTITYTYPE::TEMPSHIP);
	//ship->setTransform(Transform(vector3D(10.0, 5.0, 10.0)));
	ship->addToWorld(this->mainWorld);

	EntityPlayerClient* player = (EntityPlayerClient*)EntityManager::instance->createEntity(ENTITYTYPE::PLAYER_THIS);
	this->playerInterface.bindCharacter(player);

	player->setTransform(Transform(vector3D(0.0, 0.0, -15.0)));
	player->addToWorld(this->mainWorld);

	EntityNode* nodes = (EntityNode*)EntityManager::instance->createEntity(ENTITYTYPE::ENTITY_NODE);
	nodes->setTransform(Transform(vector3D(0.0, 15.0, 0.0)));
	nodes->addToWorld(this->mainWorld);
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
	EntityManager::instance->update();
	this->playerInterface.updatePlayerInput();
	WorldManager::instance->update(deltaTime);

	Entity* player = this->playerInterface.getBoundCharacter();
	Transform transform = this->playerInterface.getCameraTransform();
	Camera cam;
	cam.setCameraTransform(transform);
	client->window->clearBuffer();
	if (player != nullptr)
	{
		client->renderingManager->Render(player->getWorld(), &cam);
	}
	client->window->updateBuffer();
}