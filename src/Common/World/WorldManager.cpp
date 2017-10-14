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

	while (this->worlds.find(worldId) != this->worlds.end())
	{
		nextId++;
		worldId = nextId;
	}

	nextId++;
	return worldId;
}

void WorldManager::update(double deltaTime)
{
	//TODO muiltiThreadUpdate
	std::unordered_map<WorldId, World*>::iterator iter;
	for (iter = this->worlds.begin(); iter != this->worlds.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			iter->second->update(deltaTime);
		}
	}
}

World* WorldManager::createWorld(WORLDTYPE type)
{
	return this->createWorld(type, this->getNextId());
}

World* WorldManager::createWorld(WORLDTYPE type, WorldId id)
{
	World* world;
	if (this->worlds.find(id) != this->worlds.end())
	{
		return nullptr;
	}

	if (type == WORLDTYPE::SOLAR)
	{
		world = new WorldSolarSystem(id);
	}
	else
	{
		world = new World(id);
	}

	this->worlds[id] = world;

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