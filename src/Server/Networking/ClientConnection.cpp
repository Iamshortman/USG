#include "Server/Networking/ClientConnection.hpp"
#include "Common/World/WorldManager.hpp"
#include <stack>

ClientConnection::ClientConnection(RakNet::SystemAddress address)
{
	this->address = address;
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
	return "";//this->username;
}

RakNet::SystemAddress ClientConnection::getAddress()
{
	return this->address;
}

Entity* ClientConnection::getControllingEntity()
{
	return this->controllingEntity;
}

void ClientConnection::closeConnection()
{
	if (this->controllingEntity != nullptr)
	{
		this->controllingEntity->kill();
	}
}

bool ClientConnection::shouldCloseConnection()
{
	return this->shoudlCloseConnection;
}

void ClientConnection::updateEntitiesInFocus()
{
	for (auto worldId : this->worldsInFocus)
	{
		World* world = WorldManager::instance->getWorld(worldId);
		auto entities = world->getEntitiesInWorld();
		for (auto entity = entities->begin(); entity != entities->end(); entity++)
		{
			if (this->entitiesInFocus.find((*entity)->entityId) == this->entitiesInFocus.end())
			{
				this->entitiesToLoad.push((*entity)->entityId);
			}
		}
	}

	/*if (this->controllingEntity != nullptr)
	{
		World* world = this->controllingEntity->getWorld();
		auto entities = world->getEntitiesInWorld();
		for (auto entity = entities->begin(); entity != entities->end(); entity++)
		{
			if (this->entitiesInFocus.find(*entity) == this->entitiesInFocus.end())
			{
				this->entitiesToLoad.push(*entity);
			}
		}
	}*/
}