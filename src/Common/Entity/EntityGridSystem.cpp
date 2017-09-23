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

bool EntityGridSystem::hasSubWorld()
{
	return this->subWorld != nullptr;
}

World* EntityGridSystem::getSubWorld()
{
	return this->subWorld;
}

void EntityGridSystem::setSubWorld(World* world)
{
	this->removeSubWorld();

	this->subWorld = world;
	this->subWorld->setParent(this);
	this->world->addSubWorld(this->subWorld);
}

void EntityGridSystem::removeSubWorld()
{
	if (this->subWorld != nullptr)
	{
		this->world->removeSubWorld(this->subWorld);
		this->subWorld->setParent(nullptr);
		this->subWorld = nullptr;
	}
}

RigidBody* EntityGridSystem::getRigidBody()
{
	return nullptr;
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

ENTITYTYPE EntityGridSystem::getEntityType() const
{
	return ENTITYTYPE::GRIDSYSTEM;
}

