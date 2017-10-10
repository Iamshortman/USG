#include "Common/Entity/EntityCharacter.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/CapsuleShape.hpp"
#include "Common/GLM_Include.hpp"

EntityCharacter::EntityCharacter(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 100.0, new CapsuleShape(0.3, 1.8));
	this->rigidBody->setInertiaTensor(vector3D(0.0));
}

EntityCharacter::~EntityCharacter()
{
	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}
}

RigidBody* EntityCharacter::getRigidBody()
{
	return this->rigidBody;
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

Transform EntityCharacter::getTransform()
{
	return this->rigidBody->getWorldTransform();
}


void EntityCharacter::setTransform(Transform transform)
{
	this->rigidBody->setWorldTransform(transform);
}

void EntityCharacter::addToWorld(World* world)
{
	//Remove from the current world
	if (this->world != nullptr)
	{
		if (this->rigidBody != nullptr)
		{
			this->world->removeRigidBody(this->rigidBody);
		}

		this->world->removeEntityFromWorld(this);
	}

	//Set the new world even if its null
	this->world = world;

	//add to the current world
	if (this->world != nullptr)
	{
		if (this->rigidBody != nullptr)
		{
			this->world->addRigidBody(this->rigidBody);
		}

		this->world->addEntityToWorld(this);
	}
}

ENTITYTYPE EntityCharacter::getEntityType() const
{
	return ENTITYTYPE::CHARACTOR;
}

void EntityCharacter::writeNetworkPacket(BitStream* packet)
{
	packet->Write(this->entityId);
	packet->Write(this->getTransform());
	packet->Write(this->rigidBody->getLinearVelocity());
	packet->Write(this->rigidBody->getAngularVelocity());
	packet->Write(this->linearInput);
	packet->Write(this->angularInput);
}

void EntityCharacter::readNetworkPacket(BitStream* packet)
{
	packet->Read(this->transform);
	this->setTransform(this->transform);

	vector3D temp;
	packet->Read(temp);
	this->rigidBody->setLinearVelocity(temp);

	packet->Read(temp);
	this->rigidBody->setAngularVelocity(temp);

	packet->Read(this->linearInput);
	packet->Read(this->angularInput);
}