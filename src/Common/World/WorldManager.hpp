#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/World/World.hpp"

class WorldManager
{

public:
	static WorldManager* getInstance();

	~WorldManager();

	WorldId getNextId();

	void update(double deltaTime);


	World* createWorld();
	World* createWorld(WorldId worldId);

	void destroyWorld(WorldId id);

	World* getWorld(WorldId id);
	std::unordered_map<WorldId, World*>::iterator getAllWorlds();

private:
	//Instance for the Singleton design pattern;
	static WorldManager* instance;
	WorldManager();

	std::unordered_map<WorldId, World*> worlds;
	WorldId nextId = 1;
};

#endif //WORLDMANAGER_HPP