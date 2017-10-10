#include "Server/Networking/ClientConnection.hpp"
#include "Common/World/World.hpp"


ClientConnection::ClientConnection(string username)
{
	this->username = username;
}

ClientConnection::~ClientConnection()
{

}

void ClientConnection::setControllingEntity(Entity* entity)
{
	this->controllingEntity = entity;
}

string ClientConnection::getUsername()
{
	return this->username;
}

Entity* ClientConnection::getControllingEntity()
{
	return this->controllingEntity;
}

void ClientConnection::closeConnection()
{
	this->shoudlCloseConnection = true;
}

bool ClientConnection::shouldCloseConnection()
{
	return this->shoudlCloseConnection;
}

void ClientConnection::updateEntitiesInFocus()
{
	if (this->controllingEntity != nullptr)
	{
		World* world = this->controllingEntity->getWorld();

		std::set<Entity*>* entities = world->getEntitiesInWorld();

		this->entitiesInFocus.insert(entities->begin(), entities->end());
	}
	else
	{
		this->entitiesInFocus.clear();
	}

}
