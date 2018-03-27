#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/GameObject.hpp"

class GameObjectManager
{
public:
	//Instance for the Singleton design pattern;
	static GameObjectManager* instance;

	GameObjectManager();

	void update();

	GameObject* i_createGameObject();
	GameObject* i_getGameObject(GameObjectId id);
	void i_destroyGameObject(GameObjectId id);

	static GameObject* createGameObject();
	static GameObject* getGameObject(GameObjectId id);
	static void destroyGameObject(GameObjectId id);

private:
	std::unordered_map<GameObjectId, GameObject*> game_objects;
	std::stack<GameObjectId> game_objects_to_delete;

	GameObjectId getNextId();
	GameObjectId next_id = 1;
};

#endif //ENTITYMANAGER_HPP