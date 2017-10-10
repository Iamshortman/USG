#include "Client/Entity/EntityPlayerClient.hpp"

#include <algorithm>
#include "Client/Input/InputManager.hpp"
#include "Common/GLM_Include.hpp"

EntityPlayerClient::EntityPlayerClient(EntityId id)
	:EntityCharacter(id)
{

}

EntityPlayerClient::~EntityPlayerClient()
{

}

void EntityPlayerClient::update(double deltaTime)
{
	EntityCharacter::update(deltaTime);
}

void EntityPlayerClient::updatePlayerInput()
{
	this->linearInput.z = InputManager::instance->getButtonAxisCombo("DebugForwardBackward", "DebugForward", "DebugBackward");
	this->linearInput.x = InputManager::instance->getButtonAxisCombo("DebugLeftRight", "DebugLeft", "DebugRight");
	this->linearInput.y = InputManager::instance->getButtonAxisCombo("DebugUpDown", "DebugUp", "DebugDown");

	this->angularInput.x = InputManager::instance->getButtonAxisCombo("DebugPitch", "DebugPitchUp", "DebugPitchDown");
	this->angularInput.y = InputManager::instance->getButtonAxisCombo("DebugYaw", "DebugYawLeft", "DebugYawRight");
	this->angularInput.z = InputManager::instance->getButtonAxisCombo("DebugRoll", "DebugRollRight", "DebugRollLeft");
}

Transform EntityPlayerClient::getCameraTransform()
{
	return Transform(vector3D(0.0, 0.5, 0.0)).transformBy(this->getRenderTransform());
}

void EntityPlayerClient::writeNetworkPacket(BitStream* packet)
{
	EntityCharacter::writeNetworkPacket(packet);
}

void EntityPlayerClient::readNetworkPacket(BitStream* packet)
{
	EntityCharacter::readNetworkPacket(packet);
}

ENTITYTYPE EntityPlayerClient::getEntityType() const
{
	return ENTITYTYPE::PLAYER_THIS;
}