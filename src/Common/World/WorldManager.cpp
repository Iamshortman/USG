#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"

WorldManager* WorldManager::instance = nullptr;

WorldManager* WorldManager::getInstance()
{
	if (WorldManager::instance == nullptr)
	{
		WorldManager::instance = new WorldManager();
	}

	return WorldManager::instance;
}

WorldManager::WorldManager()
{
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

World* WorldManager::createWorld()
{
	return this->createWorld(this->getNextId());
}

World* WorldManager::createWorld(WorldId worldId)
{
	if (worldId == 0)
	{
		printf("Error: attempted to created a world with Id:0\n");
		return nullptr;
	}

	World* world;
	if (this->worlds.find(worldId) != this->worlds.end())
	{
		printf("Error: World with Id:%d already exsits\n", worldId);
		return nullptr;
	}


	world = new World(worldId);

	this->worlds[worldId] = world;

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
	if (this->worlds.find(id) == this->worlds.end())
	{
		return nullptr;
	}

	return worlds[id];
}

std::unordered_map<WorldId, World*>::iterator WorldManager::getAllWorlds()
{
	return this->worlds.begin();
}