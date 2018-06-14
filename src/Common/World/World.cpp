#include "Common/World/World.hpp"
#include "Common/Entity/EntityManager.hpp"

World::World(WorldId id)
:worldId(id)
{
	physicsWorld = new PhysicsWorld();
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
				EntityManager::getInstance()->destroyEntity(entity->entityId);
			}
		}
	}

}

void World::addEntityToWorld(Entity* entity)
{
	if (entitiesInWorld.find(entity) == entitiesInWorld.end())
	{
		entitiesInWorld.insert(entity);
	}
}

void World::removeEntityFromWorld(Entity* entity)
{
	if (entitiesInWorld.find(entity) != entitiesInWorld.end())
	{
		entitiesInWorld.erase(entity);
	}
}

std::set<Entity*>* World::getEntitiesInWorld()
{
	return &this->entitiesInWorld;
}

void  World::addRigidBody(RigidBody* rigidBody)
{
	this->physicsWorld->addRigidBody(rigidBody);
}

void  World::removeRigidBody(RigidBody* rigidBody)
{
	this->physicsWorld->removeRigidBody(rigidBody);
}

void World::addSubWorld(World* world)
{
	this->subWorlds.insert(world);
	world->parentWorld = this;
}

void World::removeSubWorld(World* world)
{
	this->subWorlds.erase(world);
	world->parentWorld = nullptr;
}

std::set<World*>* World::getSubWorlds()
{
	return &this->subWorlds;
}

vector3D World::getGravity()
{
	return toVec3(this->physicsWorld->dynamicsWorld->getGravity());
}

void World::setGravity(vector3D gravity)
{
	this->physicsWorld->dynamicsWorld->setGravity(toBtVec3(gravity));
}

SingleRayTestResult World::singleRayTest(vector3D startPos, vector3D endPos)
{
	return this->physicsWorld->singleRayTest(startPos, endPos);
}

SingleRayTestResult World::singleRayTestNotMe(vector3D startPos, vector3D endPos, Entity* me)
{
	return this->physicsWorld->singleRayTestNotMe(startPos, endPos, me);
}

void World::setParent(Entity* entity)
{
	this->parent = entity;
}

Entity* World::getParent()
{
	return this->parent;
}

bool World::hasParentWorld()
{
	return this->parentWorld != nullptr;
}

World* World::getParentWorld()
{
	return this->parentWorld;
}

Transform World::getWorldOffset()
{
	if (this->parent != nullptr)
	{
		return this->parent->getGlobalTransform();
	}

	return Transform();
}