#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Common/Entity/EntityCharacter.hpp"
#include "Common/Entity/EntityGridSystem.hpp"
#include "Common/Entity/EntityTempShip.hpp"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	instance = this;

	this->registerCreator(ENTITYTYPE::GRIDSYSTEM, new CreatorGridSystem());
	this->registerCreator(ENTITYTYPE::CHARACTOR, new CreatorCharacter());
	this->registerCreator(ENTITYTYPE::TEMPSHIP, new CreatorTempShip());
}

EntityId EntityManager::getNextId()
{
	EntityId entityId = nextId;

	while (this->entities.find(entityId) != this->entities.end())
	{
		nextId++;
		entityId = nextId;
	}

	nextId++;
	return entityId;
}

Entity* EntityManager::createEntity(ENTITYTYPE type)
{
	Entity* entity = this->creators[type]->create(this->getNextId());
	entities[entity->entityId] = entity;
	return entity;
}

Entity* EntityManager::createEntity(ENTITYTYPE type, EntityId entityId)
{
	if (this->entities.find(entityId) == this->entities.end())
	{
		Entity* entity = this->creators[type]->create(entityId);
		entities[entity->entityId] = entity;
		return entity;
	}

	return nullptr;
}

Entity* EntityManager::createEntityFromNetwork(BitStream* in)
{
	ENTITYTYPE type; 
	in->Read(type);

	WorldId worldId;
	in->Read(worldId);

	EntityId entityId;
	in->Read(entityId);

	Entity* entity = this->createEntity(type, entityId);
	if (entity != nullptr)
	{
		World* world = WorldManager::instance->getWorld(worldId);
		if (world != nullptr)
		{
			entity->addToWorld(world);
		}

		entity->readNetworkPacket(in);
	}
	else
	{
		printf("Error: couldn't create entity from packet\n");
	}

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

void EntityManager::registerCreator(ENTITYTYPE type, Creator* creator)
{
	this->creators[type] = creator;
}