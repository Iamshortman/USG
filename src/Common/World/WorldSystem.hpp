#ifndef WORLD_SYSTEM_HPP
#define WORLD_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Transform.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

#include <map>
#include <set>

typedef uint32_t WorldId;

#define INVALID_WORLD 0

struct WorldChangeEvent
{
	WorldChangeEvent(Entity entity, WorldId old_world, WorldId new_world) : entity(entity), old_world(old_world), new_world(new_world) {}

	Entity entity;
	WorldId old_world;
	WorldId new_world;
};

struct World
{
	World(WorldId id) : world_id(id) {}
	const WorldId world_id;
};

struct WorldHost
{
	WorldHost(WorldId id) : world_id(id) {}
	const WorldId world_id;

	std::set<Entity> entity_list;
	PhysicsWorld* physics_world;
};

class WorldSystem : public System<WorldSystem>, public Receiver<WorldSystem>
{
public:
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);

	void receive(const WorldChangeEvent& event);

	void receive(const ComponentAddedEvent<WorldHost> &event);
	void receive(const ComponentRemovedEvent<WorldHost>& event);

	void receive(const ComponentAddedEvent<World> &event);
	void receive(const ComponentRemovedEvent<World>& event);
private:
};

class WorldList
{
public:
	static WorldList* getInstance();

	~WorldList();

	bool hasWorldHost(WorldId id);
	Entity getWorldHost(WorldId id);

private:
	//Instance for the Singleton design pattern;
	static WorldList* instance;
	WorldList();

	std::map<WorldId, Entity> worlds;

	friend WorldSystem;
};

#endif //WORLD_SYSTEM_HPP