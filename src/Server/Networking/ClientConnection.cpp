#include "Server/Networking/ClientConnection.hpp"

ClientConnection::ClientConnection(RakNet::SystemAddress address)
{
	this->address = address;
}

ClientConnection::~ClientConnection()
{

}

void ClientConnection::setUsername(string username)
{
	this->username = username;
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

void ClientConnection::CloseConnection()
{
	this->closeConnection = true;
}

bool ClientConnection::shouldCloseConnection()
{
	return this->closeConnection;
}
