#include "Client/GameState/GameState.hpp"
#include "Client/Client.hpp"

GameState_Multiplayer::GameState_Multiplayer()
{
	string address = "127.0.0.1";
	/*string temp;
	printf("Enter Server Address: ");
	std::cin >> temp;

	if (temp != "")
	{
		address = temp;
	}*/

	//this->networkManager = new ClientNetworkManager(address, 60000, this);
}

GameState_Multiplayer::~GameState_Multiplayer()
{
	//delete this->networkManager;
}

void GameState_Multiplayer::update(Client* client, double deltaTime)
{
	//Update from network
	/*if (this->networkManager->isConnected == false)
	{
		//Draw Black Screen
		client->window->clearBuffer();
		client->window->updateBuffer();
		return;
	}

	this->networkManager->processPackets();

	this->playerInterface.updatePlayerInput();
	WorldManager::instance->update(deltaTime);
	EntityManager::instance->update();

	Entity* player = this->playerInterface.getBoundCharacter();
	if (player != nullptr)
	{
		PacketSend send(PacketTypes::UpdateEntity, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED);
		send.bitStream_out.Write(player->entityId);
		player->writeNetworkPacket(&send.bitStream_out);
		this->networkManager->sendPacket(send);
	}

	Transform transform = this->playerInterface.getCameraTransform();
	Camera cam;
	cam.setCameraTransform(transform);
	client->window->clearBuffer();
	if (player != nullptr)
	{
		client->renderingManager->Render(player->getWorld(), &cam);
	}
	client->window->updateBuffer();*/
}