#include "ComponentShipFlight.hpp"

#include "Common/GameObject.hpp"
#include "Common/Physics/RigidBody.hpp"

#ifdef CLIENT
	#include "Client/Input/InputManager.hpp"
#endif // CLIENT

ComponentShipFlight::ComponentShipFlight()
{

}

ComponentShipFlight::~ComponentShipFlight()
{
}

void ComponentShipFlight::update(double delta_time)
{
	if (!this->enabled || !this->parent->hasComponent<RigidBody>())
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

	Transform transform = this->parent->getLocalTransform();
	RigidBody* rigidBody = this->parent->getComponent<RigidBody>();

	//Angular Section
	vector3D angular_velocity = glm::inverse(transform.getOrientation()) * rigidBody->getAngularVelocity();

	for (int i = 0; i < 3; i++)
	{
		double current_angular_velocity = angular_velocity[i];
		double desired_angular_velocity = (angular_input[i] * this->max_angular_speed[i]) * (2 * M_PI);
		double diffrence = desired_angular_velocity - current_angular_velocity;
		if (diffrence > 0.0) //Positive angular velocity
		{
			double velocity_to_add;
			if (current_angular_velocity >= 0.0) //Regular acceleration
			{
				velocity_to_add = this->angular_acceleration[i] * delta_time  * (2 * M_PI);
			}
			else //Breaking acceleration
 			{
				velocity_to_add = this->angular_braking_acceleration[i] * delta_time  * (2 * M_PI);
			}

			if (velocity_to_add > diffrence)
			{
				velocity_to_add = diffrence;
			}

			angular_velocity[i] += velocity_to_add;
		}
		else if (diffrence < 0.0) //negitive angular velocity
		{
			double velocity_to_add;
			if (current_angular_velocity <= 0.0) //Regular acceleration
			{
				velocity_to_add = this->angular_acceleration[i] * delta_time  * (2 * M_PI);
			}
			else //Breaking acceleration
			{
				velocity_to_add = this->angular_braking_acceleration[i] * delta_time  * (2 * M_PI);
			}

			if (velocity_to_add < diffrence)
			{
				velocity_to_add = diffrence;
			}

			angular_velocity[i] -= velocity_to_add;
		}
	}

	rigidBody->setAngularVelocity(transform.getOrientation() * angular_velocity);

	vector3D linear_velocity = rigidBody->getLinearVelocity();

	//Linear Section
	for (size_t i = 0; i < 3; i++)
	{
		size_t input_direction = (this->linear_input[i] >= 0.0) ? (i * 2) : (i * 2 + 1);
		double desired_velocity = this->linear_input[i] * this->linear_max_speed[input_direction];
		double current_velocity = linear_velocity[i];
		double delta_velocity = desired_velocity - current_velocity;

		size_t movement_direction = (delta_velocity >= 0.0) ? (i * 2) : (i * 2 + 1);

		if (delta_velocity != 0.0)
		{
			double acceleration = 0.0;

			//If slowing down
			if (current_velocity > 0.0 && delta_velocity <= 0.0)
			{
				acceleration = -this->linear_acceleration[movement_direction];
				acceleration -= 0.5 * (current_velocity * current_velocity) * this->linear_braking_expo[movement_direction];

			}
			else if (current_velocity < 0.0 && delta_velocity >= 0.0)
			{
				acceleration = this->linear_acceleration[movement_direction];
				acceleration += 0.5 * (current_velocity * current_velocity) * this->linear_braking_expo[movement_direction];
			}
			else
			{
				acceleration = this->linear_acceleration[movement_direction] * this->linear_input[i];
			}

			if ((acceleration > 0.0 && acceleration > delta_velocity) || (acceleration < 0.0 && acceleration < delta_velocity))
			{
				acceleration = delta_velocity;
			}

			linear_velocity[i] += acceleration;
		}
	}

	rigidBody->setLinearVelocity(linear_velocity);

	if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}
}
