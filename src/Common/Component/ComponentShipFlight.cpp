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
	this->angular_input.z = -InputManager::getInstance()->getButtonAxisCombo("Flight_Roll", "Flight_RollLeft", "Flight_RollRight");
#endif // CLIENT

	Transform transform = this->parent->getLocalTransform();
	RigidBody* rigidBody = this->parent->getComponent<RigidBody>();
	vector3D linear_velocity = glm::inverse(transform.getOrientation()) * rigidBody->getLinearVelocity();

	bool FlightAssistEnabled = true;

	//Engine Thrust
	/*for (size_t i = 0; i < 6; i++)
	{
		double currentVelocity = getVectorDirectionValue(linear_velocity, i);
		double desiredVelocity = getVectorDirectionValue(this->linear_input, i) * this->max_linear_speed[i];
		if (desiredVelocity > 0 && currentVelocity >= 0 && desiredVelocity != currentVelocity)
		{
			double difference = desiredVelocity - currentVelocity;
			double velocity_to_add = this->linear_acceleration[i] * delta_time;

			if (velocity_to_add > difference)
			{
				velocity_to_add = difference;
			}

			linear_velocity += Directions3D[i] * velocity_to_add;
		}
	}

	//Braking Force
	for (size_t i = 0; i < 6; i++)
	{
		double currentVelocity = getVectorDirectionValue(linear_velocity, i);
		if (getVectorDirectionValue(this->linear_input, i) >= 0 && currentVelocity < 0)
		{
			double difference = 0 - currentVelocity;
			double velocity_to_add = this->linear_braking_acceleration[i] * delta_time;

			if (velocity_to_add > difference)
			{
				velocity_to_add = difference;
			}

			linear_velocity += Directions3D[i] * velocity_to_add;
		}
	}*/

	for (int i = 0; i < 3; i++)
	{
		double current_linear_velocity = linear_velocity[i];
		double desired_linear_velocity = (linear_input[i] * this->max_linear_speed[i]);
		double diffrence = desired_linear_velocity - current_linear_velocity;
		if (diffrence > 0.0) //Positive linear velocity
		{
			double velocity_to_add;
			if (current_linear_velocity >= 0.0) //Regular acceleration
			{
				velocity_to_add = this->linear_acceleration[i] * delta_time;
			}
			else //Breaking acceleration
			{
				velocity_to_add = this->linear_braking_acceleration[i] * delta_time;
				velocity_to_add -= linear_velocity[i] * this->linear_drag_const[i];
			}

			if (velocity_to_add > diffrence)
			{
				velocity_to_add = diffrence;
			}

			linear_velocity[i] += velocity_to_add;
		}
		else if (diffrence < 0.0) //negitive linear velocity
		{
			double velocity_to_add;
			if (current_linear_velocity <= 0.0) //Regular acceleration
			{
				velocity_to_add = this->linear_acceleration[i] * delta_time;
			}
			else //Breaking acceleration
			{
				velocity_to_add = this->linear_braking_acceleration[i] * delta_time;
				velocity_to_add += linear_velocity[i] * this->linear_drag_const[i];
			}

			if (velocity_to_add < diffrence)
			{
				velocity_to_add = diffrence;
			}

			linear_velocity[i] -= velocity_to_add;
		}
	}

	rigidBody->setLinearVelocity(transform.getOrientation() * linear_velocity);

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

	if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}
}
