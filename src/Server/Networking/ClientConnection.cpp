#include "Server/Networking/ClientConnection.hpp"

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

void ClientConnection::CloseConnection()
{
	this->closeConnection = true;
}

bool ClientConnection::shouldCloseConnection()
{
	return this->closeConnection;
}
