#include "Common/World/World.hpp"
#include "Common/Entity/EntityManager.hpp"

World::World(WorldId id)
:worldId(id)
{
	physicsWorld = new PhysicsWorld(this);
}

World::~World()
{
	delete physicsWorld;
}

void World::update(double deltaTime)
{
	physicsWorld->update(deltaTime);
	
	for (Entity* entity : this->entitiesInWorld)
	{
		if (entity != nullptr)
		{
			if (entity->isAlive())
			{
				entity->update(deltaTime);
			}
			else
			{
				EntityManager::instance->destroyEntity(entity->entityId);
			}
		}
	}

}

void World::addEntityToWorld(Entity* entity)
{
	if (entitiesInWorld.find(entity) == entitiesInWorld.end())
	{
		entitiesInWorld.insert(entity);

		this->addEntityRigidBody(entity);
	}
}

void World::removeEntityFromWorld(Entity* entity)
{
	if (entitiesInWorld.find(entity) != entitiesInWorld.end())
	{
		entitiesInWorld.erase(entity);

		this->removeEntityRigidBody(entity);
	}
}

std::set<Entity*>* World::getEntitiesInWorld()
{
	return &this->entitiesInWorld;
}

void  World::addEntityRigidBody(Entity* entity)
{
	if (entity->hasRigidBody())
	{
		this->physicsWorld->addRigidBody(entity->getRigidBody());
	}
}

void  World::removeEntityRigidBody(Entity* entity)
{
	if (entity->hasRigidBody())
	{
		this->physicsWorld->removeRigidBody(entity->getRigidBody());
	}
}

void World::addSubWorld(World* world)
{
	this->subWorlds.insert(world);
}

void World::removeSubWorld(World* world)
{
	this->subWorlds.erase(world);
}

std::set<World*>* World::getSubWorlds()
{
	return &this->subWorlds;
}

vector3D World::getGravity()
{
	return toVec3(physicsWorld->dynamicsWorld->getGravity());
}

void World::setGravity(vector3D gravity)
{
	physicsWorld->dynamicsWorld->setGravity(toBtVec3(gravity));
}

Transform World::getWorldOffsetMatrix()
{
	if (this->parent != nullptr)
	{
		return this->parent->getRenderTransform();
	}
	
	return Transform();
}

void World::setParent(Entity* entity)
{
	this->parent = entity;
}

Entity* World::getParent()
{
	return this->parent;
}