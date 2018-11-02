#include "Entity.hpp"

#include "Common/World/World.hpp"
#include "Common/Logger/Logger.hpp"

Entity::Entity(EntityId objectId)
	: entityId(objectId)
{
}

Entity::~Entity()
{
\
}

void Entity::update(double delta_time)
{
\
}
void Entity::setLocalTransform(Transform transform)
{
	this->local_transform = transform;
}

Transform Entity::getLocalTransform()
{
	return this->local_transform;
}

Transform Entity::getRelativeTransform()
{
	return Transform();
}

Transform Entity::getWorldTransform()
{
	return this->getLocalTransform();
}

Transform Entity::getGlobalTransform()
{
	Transform global_transform = this->getLocalTransform();

	World* temp_world = this->getWorld();
	if (temp_world != nullptr)
	{
		global_transform = global_transform.transformBy(temp_world->getWorldOffset());
	}

	return global_transform;
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
	return this->world;
}
