#include "ComponentShipFlight.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Logger/Logger.hpp"

#include <math.h>

double calculate_top_speed(double mass, double force)
{
	const double acceleration_factor = 50.0;//A ship with 1.5G of acceleration can go 750 m/s
	const double absolute_top_speed = 1500.0;
	double ship_acceleration = force / mass;
	double top_speed = ship_acceleration * acceleration_factor;

	if (top_speed > absolute_top_speed)
	{
		top_speed = absolute_top_speed;
	}
	else if (top_speed < -absolute_top_speed)
	{
		top_speed = -absolute_top_speed;
	}

	return top_speed;
}

size_t flipDirection(size_t direction)
{
	if (direction % 2 == 0)
	{
		return direction + 1;
	}

	return direction - 1;
}

ShipFlightController::ShipFlightController(Entity* entity)
	:ComponentEntity(entity)
{

}

ShipFlightController::~ShipFlightController()
{
}

void ShipFlightController::setMassProps(double mass, matrix3 inv_world_tensor)
{
}

void ShipFlightController::update(double delta_time)
{
	if (!this->enabled || this->parent_entity->getRigidBody() == nullptr)
	{
		return;
	}

	this->UpdateAngularVelocity(delta_time);
	this->UpdateLinearVelocity(delta_time);

	RigidBody* rigidBody = this->parent_entity->getRigidBody();
	if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}
}

void ShipFlightController::UpdateLinearVelocity(double delta_time)
{
	Transform transform = this->parent_entity->getLocalTransform();
	RigidBody* rigidBody = this->parent_entity->getRigidBody();
	
	vector3D linearVelocity = rigidBody->getLinearVelocity();

	double mass = rigidBody->getMass();

	for (int i = 0; i < 3; i++)
	{
		vector3D direction = transform.getDirection(i);
		double velocity = glm::dot(direction, linearVelocity);
		size_t input_direction = (this->linear_input[i] > 0) ? (i * 2) : ((i * 2) + 1); //i*2 = positive direction (i*2)+1 = negitive direction
		double force = this->linear_input[i] * this->thruster_force[input_direction];

		linearVelocity += (direction * (force / mass)  * delta_time);
	}

	//When Flight Assist is enabled the ship will attempt to stop any movement on axes not currently being used.
	if (this->flight_assist)
	{
		for (int i = 0; i < 3; i++)
		{
			if (this->linear_input[i] == 0.0)
			{
				vector3D direction = transform.getDirection(i);
				double velocity = glm::dot(direction, linearVelocity);
				if (velocity != 0.0)
				{
					double braking_sign = (velocity > 0) ? -1.0 : 1.0;
					size_t braking_direction = (velocity > 0) ? ((i * 2) + 1) : (i * 2);
					double braking_force = this->thruster_force[braking_direction] * delta_time;
					double velocity_change = (braking_force / mass) * braking_sign;

					if (velocity > 0.0 && (velocity + velocity_change) < 0.0)
					{
						velocity_change = -velocity;
					}
					else if (velocity < 0.0 && (velocity + velocity_change) > 0.0)
					{
						velocity_change = -velocity;
					}

					linearVelocity += (direction * velocity_change);
				}
			}
		}
	}
	rigidBody->setLinearVelocity(linearVelocity);
}

void ShipFlightController::UpdateAngularVelocity(double delta_time)
{
	double angular_flight_assist = true;

	Transform transform = this->parent_entity->getLocalTransform();
	RigidBody* rigidBody = this->parent_entity->getRigidBody();

	for (int i = 0; i < 3; i++)
	{
		vector3D direction = transform.getDirection(i);
		if (i == 2)
		{
			direction *= -1.0;
		}

		if (this->angular_input[i] != 0.0)
		{
			double force = this->angular_input[i] * this->thruster_torque[i];
			rigidBody->applyTorqueImpulse(direction * force * delta_time);
		}
	}

	if (angular_flight_assist && false)
	{
		for (int i = 0; i < 3; i++)
		{
			if (this->angular_input[i] == 0.0)
			{
				vector3D direction = transform.getDirection(i);
				if (i == 2)
				{
					direction *= -1.0;
				}

				double starting_velocity = glm::dot(direction, rigidBody->getAngularVelocity());

				if (starting_velocity != 0.0)
				{
					if (starting_velocity > 0.0)
					{
						rigidBody->applyTorqueImpulse(direction * - this->thruster_torque[i] * delta_time);
						double new_velocity = glm::dot(direction, rigidBody->getAngularVelocity());

						if (new_velocity < 0.0)
						{
							rigidBody->setAngularVelocity(rigidBody->getAngularVelocity() + (-direction * new_velocity * delta_time));
						}
					}
					else if (starting_velocity < 0.0)
					{
						rigidBody->applyTorqueImpulse(direction *  this->thruster_torque[i]);
						double new_velocity = glm::dot(direction, rigidBody->getAngularVelocity());

						if (new_velocity > 0.0)
						{
							rigidBody->setAngularVelocity(rigidBody->getAngularVelocity() - (direction * new_velocity) * delta_time);
						}
					}
				}
			}
		}
	}

}

double ShipFlightController::getAngularAcceleration(int axis, double input)
{
	RigidBody* rigidBody = this->parent_entity->getRigidBody();
	double torque = this->thruster_torque[axis] * abs(input); 

	vector3D tensor = toVec3(rigidBody->getRigidBody()->getInvInertiaDiagLocal());
	torque *= tensor[axis];

	return torque;
}

double ShipFlightController::getLinearAcceleration(int axis, double nput)
{
	return 0.0;
}
