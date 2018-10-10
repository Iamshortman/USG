#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Common/Logger/Logger.hpp"

#include "Common/EntityX_Include.hpp"
#include "Common/Transforms.hpp"

#include "Common/Physics/RigidBody.hpp"
#include "Common/Physics/CollisionShape.hpp"
#include "Common/Physics/PhysicsWorld.hpp"

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);

	void receive(const ComponentAddedEvent<RigidBody> &event);
	void receive(const ComponentRemovedEvent<RigidBody>& event);

	void receive(const ComponentAddedEvent<CollisionShape>& event);
	void receive(const ComponentRemovedEvent<CollisionShape>& event);

	void receive(const ComponentAddedEvent<WorldHost> &event);
	void receive(const ComponentRemovedEvent<WorldHost>& event);

	void receive(const ComponentAddedEvent<World> &event);
	void receive(const ComponentRemovedEvent<World>& event);

private:

};

class PhysicsWorldList
{
public:
	static PhysicsWorldList* getInstance();

	~PhysicsWorldList();

	bool hasPhysicsWorld(WorldId id);
	PhysicsWorld* getPhysicsWorld(WorldId id);

private:
	//Instance for the Singleton design pattern;
	static PhysicsWorldList* instance;
	PhysicsWorldList();

	std::map<WorldId, PhysicsWorld*> worlds;

	friend PhysicsSystem;
};

#endif //PHYSICS_SYSTEM_HPP
