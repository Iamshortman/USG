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

World* Entity::getWorld()
{
	return world;
}

Transform Entity::getTransform()
{
	return this->transform;
}

void Entity::setTransform(Transform transform)
{
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
	this->world->addSubWorld(this->subWorld);
}

void Entity::removeSubWorld()
{
	if (this->subWorld != nullptr)
	{
		printf("Removing a subworld, Is this a problem\n");
		this->world->removeSubWorld(this->subWorld);
		this->subWorld->setParent(nullptr);
		this->subWorld = nullptr;
	}
}

RigidBody* Entity::getRigidBody()
{
	return this->rigidBody;
}