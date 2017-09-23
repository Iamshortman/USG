#include "Common/Entity/EntityCharacter.hpp"

#include "Common/World/World.hpp"
#include "Common/Physics/CollisionShapes/CapsuleShape.hpp"

EntityCharacter::EntityCharacter(EntityId id)
	:Entity(id)
{
	this->rigidBody = new RigidBody(this, 10.0, new CapsuleShape(0.3, 1.8));
}

EntityCharacter::~EntityCharacter()
{
	if (this->rigidBody != nullptr)
	{
		delete this->rigidBody;
	}
}

RigidBody* EntityCharacter::getRigidBody()
{
	return nullptr;
}

void EntityCharacter::update(double deltaTime)
{

}

Transform EntityCharacter::getTransform()
{
	return this->rigidBody->getWorldTransform();
}


void EntityCharacter::setTransform(Transform transform)
{
	this->rigidBody->setWorldTransform(transform);
}

void EntityCharacter::addToWorld(World* world)
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

ENTITYTYPE EntityCharacter::getEntityType() const
{
	return ENTITYTYPE::CHARACTOR;
}

