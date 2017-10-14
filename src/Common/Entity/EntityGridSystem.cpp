#include "Common/Entity/EntityGridSystem.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityGridSystem::EntityGridSystem(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 10.0, new BoxShape(vector3D(0.5)));
}

EntityGridSystem::~EntityGridSystem()
{
	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}
}

void EntityGridSystem::update(double deltaTime)
{

}

Transform EntityGridSystem::getTransform()
{
	return this->rigidBody->getWorldTransform();
}


void EntityGridSystem::setTransform(Transform transform)
{
	this->rigidBody->setWorldTransform(transform);
}

void EntityGridSystem::addToWorld(World* world)
{
	//Remove from the current world
	if (this->world != nullptr)
	{
		if (this->rigidBody != nullptr)
		{
			this->world->removeRigidBody(this->rigidBody);
		}

		if (this->subWorld != nullptr)
		{
			this->world->removeSubWorld(this->subWorld);
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

		if (this->subWorld != nullptr)
		{
			this->world->addSubWorld(this->subWorld);
		}

		this->world->addEntityToWorld(this);
	}
}

ENTITYTYPE EntityGridSystem::getEntityType() const
{
	return ENTITYTYPE::GRIDSYSTEM;
}

void EntityGridSystem::writeNetworkPacket(BitStream* packet)
{
	packet->Write(this->entityId);
	packet->Write(this->getTransform());
	packet->Write(this->rigidBody->getLinearVelocity());
	packet->Write(this->rigidBody->getAngularVelocity());
}

void EntityGridSystem::readNetworkPacket(BitStream* packet)
{
	//printf("ReadPacket");
	packet->Read(this->transform);
	this->setTransform(this->transform);

	vector3D temp;
	packet->Read(temp);
	this->rigidBody->setLinearVelocity(temp);

	packet->Read(temp);
	this->rigidBody->setAngularVelocity(temp);
}

