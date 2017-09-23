#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Common/World/World.hpp"
#include "Common/World/WorldSolarSystem.hpp"

class WorldManager
{

public:
	//Instance for the Singleton design pattern;
	static WorldManager* instance;

	WorldManager();
	~WorldManager();

	WorldId getNextId();

	void update(double deltaTime);

	World* createNewWorld();
	World* createWorld(int id, WORLDTYPE type);

	WorldSolarSystem* createNewWorldSolarSystem();
	void destroyWorld(WorldId id);

	World* getWorld(WorldId id);
	std::unordered_map<WorldId, World*>::iterator getAllWorlds();

private:
	std::unordered_map<WorldId, World*> worlds;
	WorldId nextId = 1;

};

#endif //WORLDMANAGER_HPP