#include "Client/Component/DebugCameraComponent.hpp"
#include "Client/Input/InputManager.hpp"

#include "Common/GLM_Include.hpp"

DebugCameraComponent::DebugCameraComponent(double linear, double angular)
{
	this->linearSpeed = linear;
	this->angularSpeed = angular;
}

DebugCameraComponent::~DebugCameraComponent()
{

}

void DebugCameraComponent::update(double deltaTime)
{
	Entity* parent = this->getParent();

	if (parent == nullptr)
	{
		return;
	}

	double forward_backward = InputManager::instance->getAxis("DebugForwardBackward");
	double left_right = InputManager::instance->getAxis("DebugLeftRight");
	double up_down = InputManager::instance->getAxis("DebugUpDown");

	Transform trans = parent->getTransform();

	vector3D forward = trans.getForward() * forward_backward;
	vector3D left = trans.getLeft() * left_right;
	vector3D up = trans.getUp() * up_down;
	vector3D finalMove = forward + left + up;
	finalMove *= this->linearSpeed * deltaTime;

	trans.setPosition(trans.getPosition() + finalMove);

	double pitch = InputManager::instance->getAxis("DebugPitch");
	double yaw = InputManager::instance->getAxis("DebugYaw");
	double roll = InputManager::instance->getAxis("DebugRoll");

	quaternionD rotation = trans.getOrientation();

	rotation = glm::angleAxis(pitch * angularSpeed * (M_PI * 2.0) * deltaTime, trans.getLeft()) * rotation;
	rotation = glm::angleAxis(yaw * angularSpeed * (M_PI * 2.0) * deltaTime, trans.getUp()) * rotation;
	rotation = glm::angleAxis(roll * (angularSpeed * 0.5) * (M_PI * 2.0) * deltaTime, trans.getForward()) * rotation;

	trans.setOrientation(rotation);

	parent->setTransform(trans);
}