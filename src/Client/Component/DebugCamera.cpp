#include "DebugCamera.hpp"

#include "Common/GameObject.hpp"
#include "Client/Input/InputManager.hpp"

DebugCamera::DebugCamera(double linear, double angular)
{
	linearSpeed = linear;
	angularSpeed = angular;
}
void DebugCamera::update(double delta_time)
{
	if (this->enabled)
	{
		Transform transform = this->parent->getLocalTransform();
		
		vector3D position = transform.getPosition();
		position += (transform.getForward() * InputManager::getInstance()->getButtonAxisCombo("DebugForwardBackward", "DebugForward", "DebugBackward")) * this->linearSpeed * delta_time;
		position += (transform.getUp() * InputManager::getInstance()->getButtonAxisCombo("DebugUpDown", "DebugUp", "DebugDown")) * this->linearSpeed * delta_time;
		position += (transform.getLeft() * InputManager::getInstance()->getButtonAxisCombo("DebugLeftRight", "DebugLeft", "DebugRight")) * this->linearSpeed * delta_time;
		transform.setPosition(position);

		quaternionD orientation = transform.getOrientation();
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("DebugPitch", "DebugPitchUp", "DebugPitchDown") * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getLeft()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("DebugYaw", "DebugYawLeft", "DebugYawRight") * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getUp()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("DebugRoll", "DebugRollRight", "DebugRollLeft") * this->angularSpeed * (M_PI) * delta_time, transform.getForward()) * orientation;
		transform.setOrientation(orientation);

		this->parent->setLocalTransform(transform);
	}
}


