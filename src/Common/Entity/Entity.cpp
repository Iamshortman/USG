#include "Entity.hpp"

#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/World/World.hpp"
#include "Common/Logger/Logger.hpp"

Entity::Entity(EntityId objectId)
	: entityId(objectId)
{
}

Entity::~Entity()
{
	for (auto component : this->component_map)
	{
		delete component.second;
	}
}

void Entity::update(double delta_time)
{
	for (auto component : this->component_map)
	{
		component.second->update(delta_time);
	}

	I_Node::update(delta_time);
}
void Entity::setLocalTransform(Transform transform)
{
	this->local_transform = transform;

	if (this->rigidBody != nullptr && this->rigidBody->isInWorld())
	{
		this->rigidBody->setWorldTransform(this->local_transform);
	}
}

Transform Entity::getLocalTransform()
{
	if (this->rigidBody != nullptr && this->rigidBody->isInWorld())
	{
		this->local_transform = this->rigidBody->getWorldTransform();
	}

	return this->local_transform;
}

Transform Entity::getRelativeTransform()
{
	return Transform();
}

Transform Entity::getWorldTransform()
{
	/*if (this->parent_node != nullptr)
	{
		return this->getLocalTransform().transformBy(parent_node->getWorldTransform());
	}*/

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
	/*else if(this->parent_node != nullptr)
	{
		global_transform = global_transform.transformBy(parent_node->getGlobalTransform());
	}*/

	return global_transform;
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

		/*if (this->subWorld != nullptr)
		{
			this->world->removeSubWorld(this->subWorld);
		}*/

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

		/*if (this->subWorld != nullptr)
		{
			this->world->addSubWorld(this->subWorld);
		}*/

		this->world->addEntityToWorld(this);
	}
}

World* Entity::getWorld()
{
	return this->world;
}

RigidBody* Entity::addRigidBody()
{
	if (this->rigidBody == nullptr)
	{
		this->rigidBody = new SingleRigidBody(this);
		this->rigidBody->setWorldTransform(this->local_transform);

		if (this->world != nullptr)
		{
			this->world->addRigidBody(this->rigidBody);
		}
	}

	return this->rigidBody;
}

void Entity::removeRigidBody()
{
	if (this->rigidBody != nullptr)
	{
		if (this->world != nullptr)
		{
			this->world->removeRigidBody(this->rigidBody);
		}

		delete this->rigidBody;
		this->rigidBody = nullptr;
	}
}

void Entity::setSubWorld(World* world)
{
	if (this->hasSubWorld())
	{
		this->sub_world->setParent(nullptr);
	}

	this->sub_world = world;

	if (this->hasSubWorld())
	{
		this->sub_world->setParent(this);
	}
}

bool Entity::hasSubWorld()
{
	return this->sub_world != nullptr;
}

World* Entity::getSubWorld()
{
	return this->sub_world;
}
