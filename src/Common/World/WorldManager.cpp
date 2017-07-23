#include "Common/World/WorldManager.hpp"

WorldManager* WorldManager::instance = nullptr;

WorldManager::WorldManager()
{
	WorldManager::instance = this;
}

WorldManager::~WorldManager()
{
	std::hash_map<WorldId, World*>::iterator iter = this->worlds.begin();
	for (iter = this->worlds.begin(); iter != this->worlds.end(); iter++)
	{
		this->destroyWorld(iter->first);
	}
}

void WorldManager::update(double deltaTime)
{
	//TODO muiltiThreadUpdate

	std::hash_map<WorldId, World*>::iterator iter;
	for (iter = this->worlds.begin(); iter != this->worlds.end(); iter++)
	{
		iter->second->update(deltaTime);
	}
}

World* WorldManager::createNewWorld()
{
	World* world = new World(nextId);
	worlds[nextId] = world;
	nextId++;
	return world;
}

void WorldManager::destroyWorld(WorldId id)
{
	if (worlds.count(id))
	{
		delete worlds[id];
		worlds.erase(id);
	}
}

World* WorldManager::getWorld(WorldId id)
{
	return worlds[id];
}

std::hash_map<WorldId, World*>::iterator WorldManager::getAllWorlds()
{
	return this->worlds.begin();
}