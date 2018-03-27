#include "Common/GameObjectManager.hpp"

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager::GameObjectManager()
{
	instance = this;
}

void GameObjectManager::update()
{
	while (!this->game_objects_to_delete.empty())
	{
		GameObjectId gameObjectId = this->game_objects_to_delete.top();

		if (this->game_objects.count(gameObjectId))
		{
			delete this->game_objects[gameObjectId];
			this->game_objects.erase(gameObjectId);
		}

		this->game_objects_to_delete.pop();
	}
}

GameObjectId GameObjectManager::getNextId()
{
	GameObjectId game_object_id = this->next_id;

	while (this->game_objects.find(game_object_id) != this->game_objects.end())
	{
		this->next_id++;
		game_object_id = this->next_id;
	}

	this->next_id++;
	return game_object_id;
}

GameObject* GameObjectManager::i_createGameObject()
{
	GameObjectId game_object_id = this->getNextId();

	GameObject* game_object = new GameObject(game_object_id);
	this->game_objects[game_object_id] = game_object;
	return game_object;
}

GameObject* GameObjectManager::i_getGameObject(GameObjectId game_object_id)
{
	if (this->game_objects.find(game_object_id) == this->game_objects.end())
	{
		return nullptr;
	}

	return this->game_objects[game_object_id];
}

void GameObjectManager::i_destroyGameObject(GameObjectId game_object_id)
{
	this->game_objects_to_delete.push(game_object_id);
}

GameObject* GameObjectManager::createGameObject()
{
	return instance->i_createGameObject();
}

GameObject* GameObjectManager::getGameObject(GameObjectId game_object_id)
{
	return instance->i_getGameObject(game_object_id);
}

void GameObjectManager::destroyGameObject(GameObjectId game_object_id)
{
	return instance->i_destroyGameObject(game_object_id);
}