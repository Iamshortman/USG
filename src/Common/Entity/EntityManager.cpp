#include "Common/Entity/EntityManager.hpp"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	instance = this;
}

Entity* EntityManager::createNewEntity()
{
	Entity* entity = new Entity(nextId);
	entities[nextId] = entity;
	nextId++;
	return entity;
}

void EntityManager::destroyEntity(EntityId id)
{
	if (entities.count(id))
	{
		delete entities[id];
		entities.erase(id);
	}
}

Entity* EntityManager::getEntity(EntityId id)
{
	return entities[id];
}

std::hash_map<EntityId, Entity*>::iterator EntityManager::getAllEntities()
{
	return this->entities.begin();
}