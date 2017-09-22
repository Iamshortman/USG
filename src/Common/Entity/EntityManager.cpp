#include "Common/Entity/EntityManager.hpp"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	instance = this;
}

EntityId EntityManager::getNextId()
{
	EntityId entityId = nextId;
	nextId++;
	return entityId;
}

EntityGridSystem* EntityManager::createNewGridSystem()
{

	EntityGridSystem* entity = new EntityGridSystem(this->getNextId());
	entities[entity->entityId] = entity;
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

std::unordered_map<EntityId, Entity*>::iterator EntityManager::getAllEntities()
{
	return this->entities.begin();
}