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

void ShipFlightController::update(double delta_time)
{
	if (!this->enabled || this->parent_entity->getRigidBody() == nullptr)
	{
		return;
	}

	this->UpdateAngularVelocity(delta_time);
	this->UpdateLinearVelocity(delta_time);

	RigidBody* rigidBody = this->parent_entity->getRigidBody();
	/*if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}*/
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
		double current_force = this->linear_input[i] * this->thruster_force[input_direction];
		rigidBody->applyCentralImpulse(direction * current_force  * delta_time);
	}

	//Update velocity
	linearVelocity = rigidBody->getLinearVelocity();

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
					double brakeing_sign = (velocity > 0) ? -1.0 : 1.0;
					size_t brakeing_direction = (velocity > 0) ? ((i * 2) + 1) : (i * 2);
					double current_force = this->thruster_force[brakeing_direction] * delta_time;
					rigidBody->applyCentralImpulse(direction * current_force * brakeing_sign);
				}
			}
		}

		//Update velocity
		linearVelocity = glm::inverse(transform.getOrientation()) * rigidBody->getLinearVelocity();
		//Round down small velocities to zero
		for (int i = 0; i < 3; i++)
		{
			if (abs(linearVelocity[i]) <= 0.1)
			{
				linearVelocity[i] = 0.0;
			}
		}
		rigidBody->setLinearVelocity(transform.getOrientation() * linearVelocity);
	}

	linearVelocity = rigidBody->getLinearVelocity();
	//printf("X: %lf Y: %lf Z: %lf\n", linearVelocity.x, linearVelocity.y, linearVelocity.z);
	//printf("Speed: %lf\n\n", glm::length(linearVelocity));
}

void ShipFlightController::UpdateAngularVelocity(double delta_time)
{
	Transform transform = this->parent_entity->getLocalTransform();

	transform.orientation = glm::angleAxis(this->angular_input.x * this->max_angular_speed.x * (M_PI * 2.0) * delta_time, transform.getLeft()) * transform.orientation;
	transform.orientation = glm::angleAxis(this->angular_input.y * this->max_angular_speed.y * (M_PI * 2.0) * delta_time, transform.getUp()) * transform.orientation;
	transform.orientation = glm::angleAxis(this->angular_input.z * this->max_angular_speed.z * (M_PI * 1.5)* delta_time, transform.getForward()) * transform.orientation;

	this->parent_entity->setLocalTransform(transform);
}