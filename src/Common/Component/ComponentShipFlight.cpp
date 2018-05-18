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
	this->thrust_input.z = InputManager::getInstance()->getButtonAxisCombo("ForwardBackward", "Forward", "Backward");
#endif // CLIENT


	Transform transform = this->parent->getLocalTransform();
	RigidBody* rigidBody = this->parent->getComponent<RigidBody>();
	vector3D linearVelocity = glm::inverse(transform.getOrientation()) * rigidBody->getLinearVelocity();

	bool FlightAssistEnabled = true;

	double currentVelocity = 0.0;
	double desiredVelocity = 0.0;
	double difference = 0.0;

	//Forward Thrust
	currentVelocity = glm::dot(transform.getForward(), linearVelocity);
	desiredVelocity = this->spaceMaxSpeed * this->thrust_input.z;
	difference = abs(currentVelocity - desiredVelocity);
	if (currentVelocity < desiredVelocity)
	{
		if (FlightAssistEnabled || this->thrust_input.z > 0.0)
		{
			double velocitytoAdd = this->forward_thrust * delta_time;
			if (velocitytoAdd >= difference)
			{
				velocitytoAdd = difference;
			}

			linearVelocity += transform.getForward() * velocitytoAdd;
		}
	}



	rigidBody->setLinearVelocity(linearVelocity);

	if (rigidBody->getLinearVelocity() != vector3D(0.0) || rigidBody->getAngularVelocity() != vector3D(0.0))
	{
		rigidBody->Activate(true);
	}
}
