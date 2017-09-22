#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"


Entity::Entity(EntityId id)
	:entityId(id)
{

}

Entity::~Entity()
{
	//Remove from the current world
	if (world != nullptr)
	{
		world->removeEntityFromWorld(this);
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
		this->world->removeEntityFromWorld(this);
	}

	//Set the new world even if its null
	this->world = world;

	//add to the current world
	if (this->world != nullptr)
	{
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