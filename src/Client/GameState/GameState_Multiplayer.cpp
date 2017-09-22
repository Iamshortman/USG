#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"
#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"
#include "Client/Rendering/LightManager.hpp"


GameState_Multiplayer::GameState_Multiplayer()
{
	this->networkManager = new ClientNetworkManager("127.0.0.1", 60000);
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

	WorldManager::instance->update(deltaTime);

	client->window->clearBuffer();

	client->window->updateBuffer();
}