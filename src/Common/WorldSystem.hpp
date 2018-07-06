#ifndef WORLD_SYSTEM_HPP
#define WORLD_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"
#include "Common/Transform.hpp"
#include <map>


typedef uint32_t WorldId;

struct WorldChangeEvent
{
	WorldChangeEvent(entityx::Entity entity, WorldId new_world) : entity(entity), new_world(new_world) {}

	entityx::Entity entity;
	WorldId new_world;
};

struct WorldTransform
{
	WorldTransform(Transform trans, WorldId world = 0)
	{
		transform = trans;
		worldId = world;
	}
	WorldTransform(vector3D position, WorldId world = 0) : WorldTransform(Transform(position), world) {}
	WorldTransform(vector3D position, quaternionD orientation, WorldId world = 0) : WorldTransform(Transform(position, orientation), world) {}

	Transform transform;
	WorldId worldId;
};

struct SubWorld
{
	SubWorld(WorldId id) : worldId(id) {}
	const WorldId worldId;
};

class WorldSystem : public System<WorldSystem>, public Receiver<WorldSystem>
{
public:
	WorldSystem();
	~WorldSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);
	void receive(const ComponentAddedEvent<SubWorld> &event);
	void receive(const ComponentRemovedEvent<SubWorld>& event);

	void receive(const ComponentAddedEvent<WorldTransform> &event);
	void receive(const ComponentRemovedEvent<WorldTransform>& event);

	void receive(const ComponentRemovedEvent<WorldChangeEvent>& event);

	bool hasWorld(WorldId id);
	Entity getWorld(WorldId id);

private:
	std::map<WorldId, Entity> worlds;
};


#endif //WORLD_SYSTEM_HPP