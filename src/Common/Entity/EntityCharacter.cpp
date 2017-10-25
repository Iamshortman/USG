#include "Common/Entity/EntityCharacter.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/CapsuleShape.hpp"
#include "Common/GLM_Include.hpp"

EntityCharacter::EntityCharacter(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 100.0, new CapsuleShape(0.4, 1));
	this->rigidBody->setInertiaTensor(vector3D(0.0));
}

EntityCharacter::~EntityCharacter()
{

}

void EntityCharacter::update(double deltaTime)
{
	if (this->linearInput != vector3D(0.0) || this->angularInput != vector3D(0.0))
	{
		this->rigidBody->Activate(true);
	}

	if (this->inGravity)
	{

	}
	else
	{
		bool FlightAssistEnabled = true;

		Transform transform = this->getTransform();
		vector3D linearVelocity = this->rigidBody->getLinearVelocity();

		double currentVelocity = 0.0;
		double desiredVelocity = 0.0;
		double difference = 0.0;

		//Forward Backward Thrust
		currentVelocity = glm::dot(transform.getForward(), linearVelocity);
		desiredVelocity = this->spaceMaxSpeed * this->linearInput.z;
		difference = abs(currentVelocity - desiredVelocity);
		if (currentVelocity < desiredVelocity)
		{
			if (FlightAssistEnabled || this->linearInput.z > 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += transform.getForward() * velocitytoAdd;
			}
		}
		else if (currentVelocity > desiredVelocity)
		{

			if (FlightAssistEnabled || this->linearInput.z < 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += -transform.getForward() * velocitytoAdd;
			}
		}

		//Left Right Thrust
		currentVelocity = glm::dot(transform.getLeft(), linearVelocity);
		desiredVelocity = this->spaceMaxSpeed * this->linearInput.x;
		difference = abs(currentVelocity - desiredVelocity);
		if (currentVelocity < desiredVelocity)
		{
			if (FlightAssistEnabled || this->linearInput.x > 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += transform.getLeft() * velocitytoAdd;
			}
		}
		else if (currentVelocity > desiredVelocity)
		{

			if (FlightAssistEnabled || this->linearInput.x < 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += -transform.getLeft() * velocitytoAdd;
			}
		}

		//Up Down Thrust
		currentVelocity = glm::dot(transform.getUp(), linearVelocity);
		desiredVelocity = this->spaceMaxSpeed * this->linearInput.y;
		difference = abs(currentVelocity - desiredVelocity);
		if (currentVelocity < desiredVelocity)
		{
			if (FlightAssistEnabled || this->linearInput.y > 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += transform.getUp() * velocitytoAdd;
			}
		}
		else if (currentVelocity > desiredVelocity)
		{

			if (FlightAssistEnabled || this->linearInput.y < 0.0)
			{
				double velocitytoAdd = this->spaceAcceleration * deltaTime;
				if (velocitytoAdd >= difference)
				{
					velocitytoAdd = difference;
				}

				linearVelocity += -transform.getUp() * velocitytoAdd;
			}
		}

		this->rigidBody->setLinearVelocity(linearVelocity);

		quaternionD rotation = transform.getOrientation();
		rotation = glm::angleAxis(this->angularInput.x * this->spaceTurnSpeed * (M_PI * 2.0) * deltaTime, transform.getLeft()) * rotation;
		rotation = glm::angleAxis(this->angularInput.y * this->spaceTurnSpeed  * (M_PI * 2.0) * deltaTime, transform.getUp()) * rotation;
		rotation = glm::angleAxis(this->angularInput.z * (this->spaceTurnSpeed  * 0.5) * (M_PI * 2.0) * deltaTime, transform.getForward()) * rotation;
		transform.setOrientation(rotation);

		this->setTransform(transform);
	}
}

ENTITYTYPE EntityCharacter::getEntityType() const
{
	return ENTITYTYPE::CHARACTOR;
}

void EntityCharacter::writeNetworkPacket(BitStream* packet)
{
	Entity::writeNetworkPacket(packet);
	packet->Write(this->rigidBody->getLinearVelocity());
	packet->Write(this->rigidBody->getAngularVelocity());
	packet->Write(this->linearInput);
	packet->Write(this->angularInput);
}

void EntityCharacter::readNetworkPacket(BitStream* packet)
{
	Entity::readNetworkPacket(packet);

	vector3D temp;
	packet->Read(temp);
	this->rigidBody->setLinearVelocity(temp);

	packet->Read(temp);
	this->rigidBody->setAngularVelocity(temp);

	packet->Read(this->linearInput);
	packet->Read(this->angularInput);
}