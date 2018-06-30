#include "ComponentShipFlight.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Physics/RigidBody.hpp"

#ifdef CLIENT
	#include "Client/Input/InputManager.hpp"
#endif // CLIENT

ComponentShipFlight::ComponentShipFlight(Entity* entity)
	:ComponentEntity(entity)
{

}

ComponentShipFlight::~ComponentShipFlight()
{
}

void ComponentShipFlight::update(double delta_time)
{
	if (!this->enabled || this->parent_entity->getRigidBody() == nullptr)
	{
		return;
	}

#ifdef CLIENT
	this->linear_input.x = InputManager::getInstance()->getButtonAxisCombo("Flight_LeftRight", "Flight_Left", "Flight_Right");
	this->linear_input.y = InputManager::getInstance()->getButtonAxisCombo("Flight_UpDown", "Flight_Up", "Flight_Down");
	this->linear_input.z = InputManager::getInstance()->getButtonAxisCombo("Flight_ForwardBackward", "Flight_Forward", "Flight_Backward");
	this->angular_input.x = InputManager::getInstance()->getButtonAxisCombo("Flight_Pitch", "Flight_PitchUp", "Flight_PitchDown");
	this->angular_input.y = InputManager::getInstance()->getButtonAxisCombo("Flight_Yaw", "Flight_YawLeft", "Flight_YawRight");
	this->angular_input.z = -InputManager::getInstance()->getButtonAxisCombo("Flight_Roll", "Flight_RollLeft", "Flight_RollRight");
#endif // CLIENT

	this->UpdateAngularVelocity(delta_time);
	this->UpdateLinearVelocity(delta_time);

	RigidBody* rigidBody = this->parent_entity->getRigidBody();
	if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}
}

void ComponentShipFlight::UpdateLinearVelocity(double delta_time)
{
	Transform transform = this->parent_entity->getLocalTransform();
	RigidBody* rigidBody = this->parent_entity->getRigidBody();

	vector3D linear_velocity = vector3D(0.0);

	//Linear Section
	for (size_t i = 0; i < 3; i++)
	{
		size_t input_direction = (this->linear_input[i] >= 0.0) ? (i * 2) : (i * 2 + 1);
		double desired_velocity = this->linear_input[i] * this->linear_max_speed[input_direction];
		linear_velocity += transform.getDirection(i) * desired_velocity;
	}

	rigidBody->setLinearVelocity(linear_velocity);
}

void ComponentShipFlight::UpdateAngularVelocity(double delta_time)
{
	Transform transform = this->parent_entity->getLocalTransform();
	RigidBody* rigidBody = this->parent_entity->getRigidBody();

	vector3D angular_velocity = vector3D(0.0);

	for (int i = 0; i < 3; i++)
	{
		double current_angular_velocity = angular_velocity[i];
		double desired_angular_velocity = (angular_input[i] * this->max_angular_speed[i]) * (2 * M_PI);
		angular_velocity += transform.getDirection(i) * desired_angular_velocity;
	}

	rigidBody->setAngularVelocity(angular_velocity);
}
