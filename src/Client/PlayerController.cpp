#include "PlayerController.hpp"

#include "Common/Component/ComponentShipFlight.hpp"
#include "Common/Component/CharacterController.hpp"
#include "Common/Component/ShipController.hpp"

#include "Client/Component/PlayerShipController.hpp"
#include "Client/Input/InputManager.hpp"

void PlayerController::setPlayerEntity(Entity* entity)
{
	this->player_entity = entity;
}

void PlayerController::setTempPlayerEntity(Entity* entity)
{
	this->player_temp_entity = entity;
}

Camera* PlayerController::getCamera()
{
	Entity* entity = this->getCurrentEntity();
	if (entity != nullptr)
	{
		return (Camera*)entity->active_camera;
	}

	return nullptr;
}

World* PlayerController::getWorld()
{
	Entity* entity = this->getCurrentEntity();
	if (entity != nullptr)
	{
		return entity->getWorld();
	}

	return nullptr;
}

void PlayerController::applyUserInput(double delta_time)
{
	Entity* entity = this->getCurrentEntity();
	if (entity != nullptr)
	{
		if(entity->hasComponent<CharacterController>())
		{
			CharacterController* controller = entity->getComponent<CharacterController>();
			double forward = InputManager::getInstance()->getButtonAxisCombo("Debug_ForwardBackward", "Debug_Forward", "Debug_Backward");
			double left = InputManager::getInstance()->getButtonAxisCombo("Debug_LeftRight", "Debug_Left", "Debug_Right");
			controller->linear_input = vector3D(left, 0.0, forward);
			controller->jump = InputManager::getInstance()->getButtonPressed("Char_Jump");
			controller->interact = InputManager::getInstance()->getButtonPressed("Debug_Interact");

			controller->angular_input.x = InputManager::getInstance()->getButtonAxisCombo("Debug_Pitch", "Debug_PitchUp", "Debug_PitchDown", false);
			controller->angular_input.y = InputManager::getInstance()->getButtonAxisCombo("Debug_Yaw", "Debug_YawLeft", "Debug_YawRight", false);
		}

		if(entity->hasComponent<PlayerShipController>())
		{
			PlayerShipController* controller = entity->getComponent<PlayerShipController>();

			controller->player_input_linear.x = InputManager::getInstance()->getButtonAxisCombo("Debug_LeftRight", "Debug_Left", "Debug_Right");
			controller->player_input_linear.y = InputManager::getInstance()->getButtonAxisCombo("Debug_UpDown", "Debug_Up", "Debug_Down");
			controller->player_input_linear.z = InputManager::getInstance()->getButtonAxisCombo("Debug_ForwardBackward", "Debug_Forward", "Debug_Backward");

			controller->player_input_angular.x = InputManager::getInstance()->getButtonAxisCombo("Debug_Pitch", "Debug_PitchUp", "Debug_PitchDown");
			controller->player_input_angular.y = InputManager::getInstance()->getButtonAxisCombo("Debug_Yaw", "Debug_YawLeft", "Debug_YawRight");
			controller->player_input_angular.z = InputManager::getInstance()->getButtonAxisCombo("Debug_Roll", "Debug_RollLeft", "Debug_RollRight");

			controller->retro_burn_mode_input = InputManager::getInstance()->getButtonPressed("Debug_Retro");
		}

		if (entity->hasComponent<ShipController>())
		{
			ShipController* controller = entity->getComponent<ShipController>();

			controller->fire_weapon = InputManager::getInstance()->getButtonDown("Flight_Shoot");
		}
	}
}

Entity* PlayerController::getCurrentEntity()
{
	if (this->player_temp_entity != nullptr)
	{
		return this->player_temp_entity;
	}

	return this->player_entity;
}
