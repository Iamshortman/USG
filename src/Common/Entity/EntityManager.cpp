#include "Common/Entity/EntityManager.hpp"

#include "Common/Rendering/Model.hpp"

EntityManager* EntityManager::instance = nullptr;

EntityManager* EntityManager::getInstance()
{
	if (EntityManager::instance == nullptr)
	{
		EntityManager::instance = new EntityManager();
	}

	return EntityManager::instance;
}

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	while (!this->game_objects_to_delete.empty())
	{
		EntityId entity_id = this->game_objects_to_delete.top();

		if (this->game_objects.count(entity_id))
		{
			this->game_objects[entity_id]->addToWorld(nullptr);
			delete this->game_objects[entity_id];
			this->game_objects.erase(entity_id);
		}

		this->game_objects_to_delete.pop();
	}
}

EntityId EntityManager::getNextId()
{
	EntityId entity_id = this->next_id;

	while (this->game_objects.find(entity_id) != this->game_objects.end())
	{
		this->next_id++;
		entity_id = this->next_id;
	}

	this->next_id++;
	return entity_id;
}

Entity* EntityManager::createEntity()
{
	EntityId entity_id = this->getNextId();

	Entity* game_object = new Entity(entity_id);
	this->game_objects[entity_id] = game_object;
	return game_object;
}
 
Entity* EntityManager::getEntity(EntityId entity_id)
{
	if (this->game_objects.find(entity_id) == this->game_objects.end())
	{
		return nullptr;
	}

	return this->game_objects[entity_id];
}

void EntityManager::destroyEntity(EntityId entity_id)
{
	this->game_objects_to_delete.push(entity_id);
}
