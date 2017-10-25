#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"
#include "Common/World/WorldManager.hpp"

Entity::Entity(EntityId id)
	:entityId(id)
{

}

Entity::~Entity()
{
	//Remove from the current world
	if (this->world != nullptr)
	{
		this->world->removeEntityFromWorld(this);
	}

	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}

	if (this->subWorld != nullptr)
	{
		WorldManager::instance->destroyWorld(this->subWorld->worldId);
	}
}

void Entity::kill()
{
	this->alive = false;
}

void Entity::update(double deltaTime)
{

}

void Entity::addToWorld(World* world)
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

World* Entity::getWorld()
{
	return world;
}

Transform Entity::getTransform()
{
	if (this->rigidBody != nullptr)
	{
		this->transform = this->rigidBody->getWorldTransform();
	}
	
	return this->transform;
}

void Entity::setTransform(Transform transform)
{
	if (this->rigidBody != nullptr)
	{
		this->rigidBody->setWorldTransform(transform);
	}

	this->transform = transform;
}

Transform Entity::getRenderTransform()
{
	Transform transform = getTransform();

	if (this->getWorld())
	{
		transform = transform.transformBy(world->getWorldOffsetMatrix());
	}

	return transform;
}

bool Entity::hasSubWorld()
{
	return this->subWorld != nullptr;
}

World* Entity::getSubWorld()
{
	return this->subWorld;
}

void Entity::setSubWorld(World* world)
{
	this->removeSubWorld();

	this->subWorld = world;
	this->subWorld->setParent(this);

	if (this->world != nullptr)
	{
		this->world->addSubWorld(this->subWorld);
	}
}

void Entity::removeSubWorld()
{
	if (this->subWorld != nullptr)
	{
		printf("Removing a subworld, Is this a problem\n");
		if (this->world != nullptr)
		{
			this->world->removeSubWorld(this->subWorld);
		}
		this->subWorld->setParent(nullptr);
		this->subWorld = nullptr;
	}
}

RigidBody* Entity::getRigidBody()
{
	return this->rigidBody;
}

void Entity::writeNetworkPacket(BitStream* packet)
{
	if (this->world != nullptr)
	{
		packet->Write(this->world->worldId);
	}
	else
	{
		packet->Write((WorldId) 0);
	}

	packet->Write(this->getTransform());

	if (this->rigidBody != nullptr)
	{
		packet->Write(this->rigidBody->getLinearVelocity());
		packet->Write(this->rigidBody->getAngularVelocity());
	}
}

void Entity::readNetworkPacket(BitStream* packet)
{
	WorldId worldId = 0;
	packet->Read(worldId);

	if (this->world != nullptr)
	{
		if (this->world->worldId != worldId)
		{
			World* newWorld = WorldManager::instance->getWorld(worldId);
			this->addToWorld(newWorld);
		}
	}
	else
	{
		World* newWorld = WorldManager::instance->getWorld(worldId);
		this->addToWorld(newWorld);
	}

	packet->Read(this->transform);
	this->setTransform(this->transform);

	if (this->rigidBody != nullptr)
	{
		vector3D temp;
		packet->Read(temp);
		this->rigidBody->setLinearVelocity(temp);

		packet->Read(temp);
		this->rigidBody->setAngularVelocity(temp);
	}
}