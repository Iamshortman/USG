#include "Client/Entity/PlayerController.hpp"

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{

}

void PlayerController::updatePlayerInput()
{
	if (this->character != nullptr)
	{
		this->character->updatePlayerInput();
	}
}

Transform PlayerController::getCameraTransform()
{
	if (this->character != nullptr)
	{
		return this->character->getCameraTransform();
	}

	return Transform();
}

void PlayerController::bindCharacter(EntityPlayerClient* character)
{
	this->character = character;
}

void PlayerController::unbindCharacter()
{
	this->character = nullptr;
}

Entity* PlayerController::getBoundCharacter()
{
	return (Entity*) this->character;
}