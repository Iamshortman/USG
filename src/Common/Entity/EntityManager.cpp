#include "Common/Entity/EntityManager.hpp"

#include "Common/Component/ComponentModel.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Component/ComponentShipFlight.hpp"
#include "Common/Component/ComponentMass.hpp"

#include "jsoncons/json.hpp"
using jsoncons::json;

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
		EntityId gameObjectId = this->game_objects_to_delete.top();

		if (this->game_objects.count(gameObjectId))
		{
			delete this->game_objects[gameObjectId];
			this->game_objects.erase(gameObjectId);
		}

		this->game_objects_to_delete.pop();
	}
}

EntityId EntityManager::getNextId()
{
	EntityId game_object_id = this->next_id;

	while (this->game_objects.find(game_object_id) != this->game_objects.end())
	{
		this->next_id++;
		game_object_id = this->next_id;
	}

	this->next_id++;
	return game_object_id;
}

Entity* EntityManager::createEntity()
{
	EntityId game_object_id = this->getNextId();

	Entity* game_object = new Entity(game_object_id);
	this->game_objects[game_object_id] = game_object;
	return game_object;
}
 
Entity* EntityManager::getEntity(EntityId game_object_id)
{
	if (this->game_objects.find(game_object_id) == this->game_objects.end())
	{
		return nullptr;
	}

	return this->game_objects[game_object_id];
}

void EntityManager::destroyEntity(EntityId game_object_id)
{
	this->game_objects_to_delete.push(game_object_id);
}
