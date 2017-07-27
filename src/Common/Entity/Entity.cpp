#include "Common/Entity/Entity.hpp"
#include "Common/World/World.hpp"


Entity::Entity(EntityId id)
	:entityId(id)
{

}

Entity::~Entity()
{
	if (rigidBody != nullptr)
	{
		delete rigidBody;
	}
	//delete RigidBody;

	//Remove from the current world
	if (world != nullptr)
	{
		for (auto iterator : components)
		{
			iterator.second->removeFromWorld(world);
		}

		world->removeEntityFromWorld(this);
	}

	//Not a huge fan of the auto keyword
	//But what are you going to do
	for (auto iterator : components)
	{
		delete iterator.second;
	}
}

void Entity::kill()
{
	this->alive = false;
}

void Entity::update(double deltaTime)
{
	//Not a huge fan of the auto keyword
	//But what are you going to do
	for (auto iterator : components)
	{
		iterator.second->update(deltaTime);
	}
}

void Entity::addToWorld(World* world)
{
	//Remove from the current world
	if (this->world != nullptr)
	{
		for (auto iterator : components)
		{
			iterator.second->removeFromWorld(world);
		}

		this->world->removeEntityFromWorld(this);
	
		if (this->subWorld != nullptr)
		{
			this->world->removeSubWorld(this->subWorld);
		}
	}

	//Set the new world even if its null
	this->world = world;

	//add to the current world
	if (this->world != nullptr)
	{
		this->world->addEntityToWorld(this);

		for (auto iterator : components)
		{
			iterator.second->addToWorld(world);
		}

		if (this->subWorld != nullptr)
		{
			this->world->addSubWorld(this->subWorld);
		}

	}
}

World* Entity::getWorld()
{
	return world;
}

vector3D Entity::getScale()
{
	return transform.getScale();
}

void Entity::setScale(vector3D scale)
{
	transform.setScale(scale);
}

Transform Entity::getTransform()
{
	if (this->rigidBody != nullptr)
	{
		return this->rigidBody->getWorldTransform(this->transform.scale);
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

bool Entity::hasComponent(std::string componentName)
{
	return this->components.find(componentName) != this->components.end();
}

Component* Entity::getComponent(std::string componentName)
{
	return this->components[componentName];
}

void Entity::addComponent(std::string componentName, Component* component)
{
	this->components[componentName] = component;
	component->setParent(this);

	if (this->world != nullptr)
	{
		component->addToWorld(this->world);
	}
}

void Entity::removeComponent(std::string componentName)
{
	if (this->hasComponent(componentName))
	{
		if (this->world != nullptr)
		{
			components[componentName]->removeFromWorld(this->world);
		}

		delete components[componentName];
		components.erase(componentName);
	}
}

Transform Entity::getRenderTransform()
{
	Transform worldTransform = world->getWorldOffsetMatrix();
	Transform transform = getTransform();

	transform.setOrientation(worldTransform.getOrientation() * transform.getOrientation());
	transform.setPosition(worldTransform.getPosition() + (worldTransform.getOrientation() * transform.getPosition()));

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
		this->world->removeSubWorld(this->subWorld);
		this->subWorld->setParent(nullptr);
		this->subWorld = nullptr;
	}
}

void Entity::createRigidBody(double mass, CollisionShape* shape)
{
	if (shape == nullptr)
	{
		this->rigidBody = new RigidBody(this, mass);
	}
	else
	{
		this->rigidBody = new RigidBody(this, mass, shape);
	}

	this->rigidBody->setWorldTransform(this->transform);

	if (this->world != nullptr)
	{
		this->world->addEntityRigidBody(this);
	}
}

void Entity::removeRigidBody()
{
	if (this->world != nullptr)
	{
		this->world->removeEntityFromWorld(this);
	}

	delete this->rigidBody;
	this->rigidBody = nullptr;
}

bool Entity::hasRigidBody()
{
	return this->rigidBody != nullptr;
}

RigidBody* Entity::getRigidBody()
{
	return this->rigidBody;
}
