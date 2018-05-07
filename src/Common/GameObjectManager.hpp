#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/GameObject.hpp"
#include "Common/Types.hpp"

class GameObjectManager
{
public:
	static GameObjectManager* getInstance();

	void update();

	GameObject* createGameObject();
	GameObject* getGameObject(GameObjectId id);
	void destroyGameObject(GameObjectId id);

	GameObject* createGameObjectFromJson(string file);

private:
	//Instance for the Singleton design pattern;
	static GameObjectManager* instance;
	GameObjectManager();

	std::unordered_map<GameObjectId, GameObject*> game_objects;
	std::stack<GameObjectId> game_objects_to_delete;

	GameObjectId getNextId();
	GameObjectId next_id = 1;
};

#endif //ENTITYMANAGER_HPP