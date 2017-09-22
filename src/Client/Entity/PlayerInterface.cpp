#include "Client/Entity/PlayerInterface.hpp"

PlayerInterface::PlayerInterface()
{

}

PlayerInterface::~PlayerInterface()
{

}

void PlayerInterface::updatePlayerInput()
{

}

Transform PlayerInterface::getCameraTransform()
{
	return Transform();
}

void PlayerInterface::bindCharacter(Entity* character)
{
	this->character = character;
}

void PlayerInterface::unbindCharacter()
{
	this->character = nullptr;
}
