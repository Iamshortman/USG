#include "Server/Server.hpp"

Server* Server::instance = nullptr;

Server::Server()
{
	Server::instance = this;
}

Server::~Server()
{

}

void Server::update(double deltaTime)
{

}

void Server::exitGame()
{
	shouldClose = true;
}
 
const bool Server::getShouldClose()
{
	return shouldClose;
}
