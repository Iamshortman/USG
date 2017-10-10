#include "Common/Entity/EntityTempShip.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/BoxShape.hpp"

EntityTempShip::EntityTempShip(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 100.0, new BoxShape(vector3D(1.5)));
}

EntityTempShip::~EntityTempShip()
{
	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}
}

bool EntityTempShip::hasSubWorld()
{
	return this->subWorld != nullptr;
}

World* EntityTempShip::getSubWorld()
{
	return this->subWorld;
}

void EntityTempShip::setSubWorld(World* world)
{
	this->removeSubWorld();

	this->subWorld = world;
	this->subWorld->setParent(this);
	this->world->addSubWorld(this->subWorld);
}

void EntityTempShip::removeSubWorld()
{
	if (this->subWorld != nullptr)
	{
		this->world->removeSubWorld(this->subWorld);
		this->subWorld->setParent(nullptr);
		this->subWorld = nullptr;
	}
}

RigidBody* EntityTempShip::getRigidBody()
{
	return this->rigidBody;
}

void EntityTempShip::update(double deltaTime)
{

}

Transform EntityTempShip::getTransform()
{
	return this->rigidBody->getWorldTransform();
}


void EntityTempShip::setTransform(Transform transform)
{
	this->rigidBody->setWorldTransform(transform);
}

void EntityTempShip::addToWorld(World* world)
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

ENTITYTYPE EntityTempShip::getEntityType() const
{
	return ENTITYTYPE::TEMPSHIP;
}

void EntityTempShip::writeNetworkPacket(BitStream* packet)
{
	packet->Write(this->entityId);
	packet->Write(this->getTransform());
	packet->Write(this->rigidBody->getLinearVelocity());
	packet->Write(this->rigidBody->getAngularVelocity());
}

void EntityTempShip::readNetworkPacket(BitStream* packet)
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

