#include "DebugCamera.hpp"

#include "Common/Entity/Entity.hpp"
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
		Transform transform = this->parent_entity->getLocalTransform();
		
		vector3D position = transform.getPosition();
		position += (transform.getForward() * InputManager::getInstance()->getButtonAxisCombo("Debug_ForwardBackward", "Debug_Forward", "Debug_Backward")) * this->linearSpeed * delta_time;
		position += (transform.getUp() * InputManager::getInstance()->getButtonAxisCombo("Debug_UpDown", "Debug_Up", "Debug_Down")) * this->linearSpeed * delta_time;
		position += (transform.getLeft() * InputManager::getInstance()->getButtonAxisCombo("Debug_LeftRight", "Debug_Left", "Debug_Right")) * this->linearSpeed * delta_time;
		transform.setPosition(position);

		quaternionD orientation = transform.getOrientation();
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Pitch", "Debug_PitchUp", "Debug_PitchDown") * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getLeft()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Yaw", "Debug_YawLeft", "Debug_YawRight") * this->angularSpeed * (M_PI * 2.0) * delta_time, transform.getUp()) * orientation;
		orientation = glm::angleAxis(InputManager::getInstance()->getButtonAxisCombo("Debug_Roll", "Debug_RollRight", "Debug_RollLeft") * this->angularSpeed * (M_PI) * delta_time, transform.getForward()) * orientation;
		transform.setOrientation(orientation);

		this->parent_entity->setLocalTransform(transform);
	}
}


