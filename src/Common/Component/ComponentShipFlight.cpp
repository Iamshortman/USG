#include "ComponentShipFlight.hpp"

#include "Common/Entity/Entity.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Logger/Logger.hpp"

#include <math.h>

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

	for (int i = 0; i < 3; i++)
	{
		vector3D direction = transform.getDirection(i);
		double velocity = glm::dot(direction, linearVelocity);
		size_t input_direction = (this->linear_input[i] > 0) ? (i * 2) : ((i * 2) + 1); //i*2 = positive direction (i*2)+1 = negitive direction
		double force = this->linear_input[i] * this->thruster_force[input_direction];

		rigidBody->applyCentralImpulse(direction * force * delta_time);
	}
}

void ShipFlightController::UpdateAngularVelocity(double delta_time)
{
	double angular_flight_assist = true;

	Transform transform = this->parent_entity->getLocalTransform();
	RigidBody* rigidBody = this->parent_entity->getRigidBody();

	for (int i = 0; i < 3; i++)
	{
		vector3D direction = transform.getDirection(i);

		if (this->angular_input[i] != 0.0)
		{
			double force = this->angular_input[i] * this->thruster_torque[i];
			rigidBody->applyTorqueImpulse(direction * force * delta_time);
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
