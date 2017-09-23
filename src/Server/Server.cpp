#include "Server/Server.hpp"

#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"


Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;

	networkManager = new ServerNetworkManager(32, 60000);

	rootWorlds.push_back(WorldManager::instance->createNewWorldSolarSystem());

	//Spawn some stuff for testing
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			EntityGridSystem* ship = EntityManager::instance->createNewGridSystem();
			ship->setTransform(Transform(vector3D(x * 2.0, 0.0, y * 2.0)));

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

	//client->setControllingEntity();

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