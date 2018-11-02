#include "Common/World/World.hpp"
#include "Common/Entity/EntityManager.hpp"

#include <algorithm>

World::World(WorldId id)
:worldId(id)
{

}

World::~World()
{

}

void World::update(double deltaTime)
{

	
	/*for (Entity* entity : this->entitiesInWorld)
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
	}*/
	for (int i = 0; i < this->entitiesInWorld.size(); i++)
	{
		Entity* entity = this->entitiesInWorld[i];
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
	auto it = find(this->entitiesInWorld.begin(), this->entitiesInWorld.end(), entity);
	if (it == this->entitiesInWorld.end())
	{
		for (int i = 0; i < this->entitiesInWorld.size(); i++)
		{
			if (this->entitiesInWorld[i] == nullptr)
			{
				this->entitiesInWorld[i] = entity;
				return;
			}
		}

		//No empty spots
		this->entitiesInWorld.push_back(entity);
	}
}

void World::removeEntityFromWorld(Entity* entity)
{
	for (int i = 0; i < this->entitiesInWorld.size(); i++)
	{
		if (this->entitiesInWorld[i] == entity)
		{
			this->entitiesInWorld[i] = nullptr;
			return;
		}
	}
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