#include "WorldSystem.hpp"

#include "Common/Logger/Logger.hpp"

WorldSystem::WorldSystem()
{
}

WorldSystem::~WorldSystem()
{
}

void WorldSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
}

void WorldSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<WorldChangeEvent>(*this);

	event_manager.subscribe<ComponentAddedEvent<WorldHost>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<WorldHost>>(*this);

	event_manager.subscribe<ComponentAddedEvent<World>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<World>>(*this);
}

void WorldSystem::receive(const WorldChangeEvent& event)
{
	Entity entity = event.entity;

	if (entity.has_component<World>())
	{
		entity.remove<World>();
	}

	entity.assign<World>(event.new_world);
}

void WorldSystem::receive(const ComponentAddedEvent<WorldHost>& event)
{
	Entity entity = event.entity;

	if (WorldList::getInstance()->hasWorldHost(event.component->world_id))
	{
		Logger::getInstance()->logError("World %d already exists\n", event.component->world_id);
		entity.remove<WorldHost>();

		return;
	}

	WorldList::getInstance()->worlds[event.component->world_id] = entity;

	entity.component<WorldHost>()->physics_world = new PhysicsWorld();
	entity.component<WorldHost>()->physics_world->dynamicsWorld->setGravity(btVector3(0.0, -0.98, 0.0));
}

void WorldSystem::receive(const ComponentRemovedEvent<WorldHost>& event)
{
	if (WorldList::getInstance()->hasWorldHost(event.component->world_id))
	{
		WorldList::getInstance()->worlds.erase(event.component->world_id);
	}

	//Destory all entities
	for (Entity child : event.component->entity_list)
	{
		child.destroy();
	}

	Entity entity = event.entity;
	delete entity.component<WorldHost>()->physics_world;
}

void WorldSystem::receive(const ComponentAddedEvent<World>& event)
{
	Entity entity = event.entity;

	if (WorldList::getInstance()->hasWorldHost(event.component->world_id))
	{
		Entity world_host = WorldList::getInstance()->getWorldHost(event.component->world_id);
		ComponentHandle<WorldHost> world = world_host.component<WorldHost>();
		world->entity_list.insert(entity);
	}
	else
	{
		Logger::getInstance()->logError("WorldAdd: World %d doesn't exists\n", event.component->world_id);
		entity.remove<World>();
	}
}

void WorldSystem::receive(const ComponentRemovedEvent<World>& event)
{
	Entity entity = event.entity;

	if (WorldList::getInstance()->hasWorldHost(event.component->world_id))
	{
		Entity world_host = WorldList::getInstance()->getWorldHost(event.component->world_id);
		ComponentHandle<WorldHost> world = world_host.component<WorldHost>();
		world->entity_list.erase(entity);
	}
}

WorldList* WorldList::instance = nullptr;

WorldList* WorldList::getInstance()
{
	if (WorldList::instance == nullptr)
	{
		WorldList::instance = new WorldList();
	}

	return WorldList::instance;
}

WorldList::~WorldList()
{

}

bool WorldList::hasWorldHost(WorldId id)
{
	return this->worlds.find(id) != this->worlds.end();
}

Entity WorldList::getWorldHost(WorldId id)
{
	if (this->hasWorldHost(id))
	{
		return this->worlds[id];
	}

	return Entity(); //Should be invalid
}

WorldList::WorldList()
{

}