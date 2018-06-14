#include "Entity.hpp"

#include "Common/World/World.hpp"
#include "Common/Logger/Logger.hpp"

Entity::Entity(EntityId objectId)
	: entityId(objectId)
{
}

Entity::~Entity()
{

}

void Entity::update(double delta_time)
{
	for (auto component : this->component_map)
	{
		component.second->update(delta_time);
	}

}

void Entity::setLocalTransform(Transform transform)
{
	this->local_transform = transform;
}

Transform Entity::getLocalTransform()
{
	return this->local_transform;
}

Transform Entity::getGlobalTransform()
{
	Transform global_transform = this->getLocalTransform();

	return global_transform;
}
