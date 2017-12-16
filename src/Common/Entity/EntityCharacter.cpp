#include "Common/Entity/EntityCharacter.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/CapsuleShape.hpp"
#include "Common/GLM_Include.hpp"

EntityCharacter::EntityCharacter(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, RIGIDBODYTYPE::SINGLE);
	this->rigidBody->setMass(100.0);
	this->rigidBody->setCollisionShape(new CapsuleShape(0.4, 1));
	this->rigidBody->setInertiaTensor(vector3D(0.0));
}

EntityCharacter::~EntityCharacter()
{

}

void EntityCharacter::update(double deltaTime)
{
	//TODO: match eyePos;
	if (this->interact)
	{
		double rayDistance = 3.0;
		Transform eyePos = this->getEyeTransform();
		vector3D startPos = eyePos.getPosition();
		vector3D endPos = startPos + (eyePos.getForward() * rayDistance);
		SingleRayTestResult result = this->world->singleRayTest(startPos, endPos);

		if (result.hasHit)
		{
			if (result.entity != nullptr)
			{
				/*if (result.entity->hasSubWorld())
				{
					World* world = result.entity->getSubWorld();
					EntityManager::instance->ChangeWorld(this, world);
					printf("change world\n");
				}*/

				Transform transform = result.entity->getTransform();
				vector3D inversePos = transform.getPosition() * -1.0; //Inverse position
				quaternionD inverseRot = glm::inverse(transform.getOrientation()); //Inverse orientation

				vector3D localPos = result.hitPosition + inversePos;
				localPos = inverseRot * localPos;

				vector3D localNorm = inverseRot * result.hitNormal;

				result.entity->interactRay(this, startPos + inversePos, localPos, localNorm, result.bodyId);
			}
		}
	}

	if (this->linearInput != vector3D(0.0) || this->angularInput != vector3D(0.0))
	{
		this->rigidBody->Activate(true);
	}

	if (this->world->getGravity() == vector3D(0.0))
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

		this->headPitch = 0.0;
	}
	else
	{
		Transform trans = this->getTransform();

		quaternionD rotation = trans.getOrientation();
		rotation = glm::angleAxis(this->angularInput.y * this->groundTurnSpeed  * (M_PI * 2.0) * deltaTime, trans.getUp()) * rotation;
		trans.setOrientation(rotation);

		this->setTransform(trans);

		this->headPitch += this->angularInput.x * this->groundTurnSpeed * (M_PI * 2.0) * deltaTime;

		if (this->headPitch > (M_PI / 2.0))
		{
			this->headPitch = (M_PI / 2.0);
		}
		else if (this->headPitch < -(M_PI / 2.0))
		{
			this->headPitch = -(M_PI / 2.0);
		}
	}
}

Transform EntityCharacter::getEyeTransform()
{
	Transform trans = Transform(vector3D(0.0, 0.5, 0.0)).transformBy(this->getTransform());
	trans.setOrientation(glm::angleAxis(this->headPitch, trans.getLeft()) * trans.getOrientation());

	return trans;
}

ENTITYTYPE EntityCharacter::getEntityType() const
{
	return ENTITYTYPE::CHARACTOR;
}

void EntityCharacter::writeNetworkPacket(BitStream* packet)
{
	Entity::writeNetworkPacket(packet);
	packet->Write(this->linearInput);
	packet->Write(this->angularInput);

	this->interact ? packet->Write1() : packet->Write0();
}

void EntityCharacter::readNetworkPacket(BitStream* packet)
{
	Entity::readNetworkPacket(packet);

	vector3D temp;
	packet->Read(this->linearInput);
	packet->Read(this->angularInput);

	this->interact = packet->ReadBit();
}