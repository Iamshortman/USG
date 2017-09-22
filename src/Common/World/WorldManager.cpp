#include "Common/World/WorldManager.hpp"

WorldManager* WorldManager::instance = nullptr;

WorldManager::WorldManager()
{
	WorldManager::instance = this;
}

WorldManager::~WorldManager()
{
	std::unordered_map<WorldId, World*>::iterator iter = this->worlds.begin();
	for (iter = this->worlds.begin(); iter != this->worlds.end(); iter++)
	{
		this->destroyWorld(iter->first);
	}
}

WorldId WorldManager::getNextId()
{
	WorldId worldId = nextId;
	nextId++;
	return worldId;
}

void WorldManager::update(double deltaTime)
{
	//TODO muiltiThreadUpdate
	std::unordered_map<WorldId, World*>::iterator iter;
	for (iter = this->worlds.begin(); iter != this->worlds.end(); iter++)
	{
		iter->second->update(deltaTime);
	}
}

World* WorldManager::createNewWorld()
{
	World* world = new World(this->getNextId());
	worlds[world->worldId] = world;
	return world;
}

WorldSolarSystem* WorldManager::createNewWorldSolarSystem()
{
	WorldSolarSystem* world = new WorldSolarSystem(this->getNextId());
	worlds[world->worldId] = world;
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

std::unordered_map<WorldId, World*>::iterator WorldManager::getAllWorlds()
{
	return this->worlds.begin();
}