#include "Common/World/WorldManager.hpp"
#include "Common/Entity/EntityManager.hpp"


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

World* WorldManager::createWorld(WORLDTYPE type, WorldId worldId)
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

	if (type == WORLDTYPE::SOLAR)
	{
		world = new WorldSolarSystem(worldId);
	}
	else
	{
		world = new World(worldId);
	}

	this->worlds[worldId] = world;

	return world;
}

World* WorldManager::createWorldFromNetwork(BitStream* in)
{
	WORLDTYPE type;
	WorldId id;
	EntityId parentId;

	in->Read(type);
	in->Read(id);
	in->Read(parentId);

	World* world = WorldManager::instance->createWorld(type, id);

	if (world != nullptr && parentId != 0)
	{
		Entity* entity = EntityManager::instance->getEntity(parentId);
		if (entity != nullptr)
		{
			entity->setSubWorld(world);
		}
	}

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