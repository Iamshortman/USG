#include "Common/Entity/EntityManager.hpp"

#include "Common/World/WorldManager.hpp"

#include "Common/Entity/EntityCharacter.hpp"
#include "Common/Entity/EntityNode.hpp"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
	instance = this;

	this->registerCreator(ENTITYTYPE::CHARACTOR, new CreatorCharacter());

	this->registerCreator(ENTITYTYPE::ENTITY_NODE, new CreatorEntityNode());
}

void EntityManager::update()
{
	while (!this->entities_to_delete.empty())
	{
		EntityId entityId = this->entities_to_delete.top();

		if (entities.count(entityId))
		{
			entities[entityId]->addToWorld(nullptr);

			delete entities[entityId];
			this->entities.erase(entityId);
		}


		this->entities_to_delete.pop();
	}

	while (!this->entities_world_change.empty())
	{
		WorldChange change = this->entities_world_change.top();

		if (change.entity != nullptr)
		{
			change.entity->addToWorld(change.newWorld);
			change.entity->setTransform(change.newTrans);
		}

		this->entities_world_change.pop();
	}
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
	return this->createEntity(type, this->getNextId());
}

Entity* EntityManager::createEntity(ENTITYTYPE type, EntityId entityId)
{
	if (entityId == 0)
	{
		printf("Error: attempted to created a entity with Id:0\n");
		return nullptr;
	}

	if (this->entities.find(entityId) == this->entities.end())
	{
		Entity* entity = this->creators[type]->create(entityId);
		entities[entity->entityId] = entity;
		return entity;
	}
	else
	{
		printf("Error: Entity with Id:%d already exsits\n", entityId);
	}

	return nullptr;
}

Entity* EntityManager::createEntityFromNetwork(BitStream* in)
{
	ENTITYTYPE type;
	WorldId worldId = 0;
	EntityId entityId = 0;
	WorldId subworldId = 0;

	in->Read(type);
	in->Read(entityId);
	in->Read(worldId);
	in->Read(subworldId);

	Entity* entity = this->createEntity(type, entityId);
	if (entity != nullptr)
	{
		if (subworldId != 0)
		{
			World* subworld = WorldManager::instance->getWorld(subworldId);
			if (subworld != nullptr)
			{
				entity->setSubWorld(subworld);
			}
		}

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

Entity* EntityManager::getEntity(EntityId entityId)
{
	if (this->entities.find(entityId) == this->entities.end())
	{
		return nullptr;
	}

	return this->entities[entityId];
}

void EntityManager::destroyEntity(EntityId entityId)
{
	this->entities_to_delete.push(entityId);
}

void EntityManager::ChangeWorld(Entity* entity, World* newWorld, Transform newTrans)
{
	this->entities_world_change.push({entity, newWorld, newTrans});
}

std::unordered_map<EntityId, Entity*>::iterator EntityManager::getAllEntities()
{
	return this->entities.begin();
}


void EntityManager::registerCreator(ENTITYTYPE type, Creator* creator)
{
	this->creators[type] = creator;
}