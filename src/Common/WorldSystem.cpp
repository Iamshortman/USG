#include "WorldSystem.hpp"

#include "Common/Logger/Logger.hpp"

WorldSystem::WorldSystem()
{
}

WorldSystem::~WorldSystem()
{
}

void WorldSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
{
}

void WorldSystem::configure(entityx::EventManager & event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<SubWorld>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<SubWorld>>(*this);

	event_manager.subscribe<ComponentAddedEvent<WorldTransform>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<WorldTransform>>(*this);
}

void WorldSystem::receive(const ComponentAddedEvent<SubWorld>& event)
{
	Entity entity = event.entity;

	if (this->hasWorld(event.component->worldId))
	{
		Logger::getInstance()->logError("World %d already exists\n", event.component->worldId);
		entity.remove<SubWorld>();

		return;
	}

	worlds[event.component->worldId] = entity;
}

void WorldSystem::receive(const ComponentRemovedEvent<SubWorld>& event)
{
	if (this->hasWorld(event.component->worldId))
	{
		worlds.erase(event.component->worldId);
	}
}

bool WorldSystem::hasWorld(WorldId id)
{
	return this->worlds.find(id) != this->worlds.end();
}

Entity WorldSystem::getWorld(WorldId id)
{
	if (this->hasWorld(id))
	{
		return this->worlds[id];
	}

	return Entity(); //Should be invalid
}

void WorldSystem::receive(const ComponentAddedEvent<WorldTransform>& event)
{

}

void WorldSystem::receive(const ComponentRemovedEvent<WorldTransform>& event)
{

}