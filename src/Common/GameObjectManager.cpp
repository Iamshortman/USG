#include "Common/GameObjectManager.hpp"

#include "jsoncons/json.hpp"
using jsoncons::json;

GameObjectManager* GameObjectManager::instance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
	if (GameObjectManager::instance == nullptr)
	{
		GameObjectManager::instance = new GameObjectManager();
	}

	return GameObjectManager::instance;
}

GameObjectManager::GameObjectManager()
{
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

GameObject* GameObjectManager::createGameObject()
{
	GameObjectId game_object_id = this->getNextId();

	GameObject* game_object = new GameObject(game_object_id);
	this->game_objects[game_object_id] = game_object;
	return game_object;
}

GameObject* GameObjectManager::getGameObject(GameObjectId game_object_id)
{
	if (this->game_objects.find(game_object_id) == this->game_objects.end())
	{
		return nullptr;
	}

	return this->game_objects[game_object_id];
}

void GameObjectManager::destroyGameObject(GameObjectId game_object_id)
{
	this->game_objects_to_delete.push(game_object_id);
}

GameObject * GameObjectManager::createGameObjectFromJson(string file)
{
	json json_file = json::parse_file(file);

	//if(json_file)



	return nullptr;
}
