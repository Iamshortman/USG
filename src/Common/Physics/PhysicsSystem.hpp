#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Common/Logger/Logger.hpp"

#include "Common/EntityX_Include.hpp"
#include "Common/Physics/SingleRigidBody.hpp"
#include "Common/Transforms.hpp"

#include "Common/Physics/PhysicsWorld.hpp"

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);
	void receive(const ComponentAddedEvent<SingleRigidBody> &event);
	void receive(const ComponentRemovedEvent<SingleRigidBody>& event);

	void receive(const ComponentAddedEvent<CollisionShape>& event);
	void receive(const ComponentRemovedEvent<CollisionShape>& event);


private:
	PhysicsWorld* temp_world;
};

PhysicsSystem::PhysicsSystem()
{
	this->temp_world = new PhysicsWorld();
	this->temp_world->dynamicsWorld->setGravity(btVector3(0.0, -0.98, 0.0));
}

PhysicsSystem::~PhysicsSystem()
{
	delete this->temp_world;
}

inline void PhysicsSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
	this->temp_world->update(dt);
}

inline void PhysicsSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<SingleRigidBody>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<SingleRigidBody>>(*this);

	event_manager.subscribe<ComponentAddedEvent<CollisionShape>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<CollisionShape>>(*this);
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<SingleRigidBody>& event)
{
	Entity entity = event.entity;
	ComponentHandle<SingleRigidBody> rigid_body = event.component;

	if (entity.has_component<WorldTransform>())
	{
		ComponentHandle<WorldTransform> transform = entity.component<WorldTransform>();
		this->temp_world->addRigidBody(rigid_body.get());

		Transform trans = transform->transform;

		rigid_body->setWorldTransform(trans);
	}
	else
	{
		Logger::getInstance()->logError("No Transform on Entity #%d\n", entity.id().id());
	}
}

inline void PhysicsSystem::receive(const ComponentRemovedEvent<SingleRigidBody>& event)
{
	Entity entity = event.entity;
	ComponentHandle<SingleRigidBody> rigid_body = event.component;

	if (entity.has_component<WorldTransform>())
	{
		ComponentHandle<WorldTransform> transform = entity.component<WorldTransform>();
		transform->transform = rigid_body->getWorldTransform();
	}

	this->temp_world->removeRigidBody(rigid_body.get());
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<CollisionShape> shape = event.component;

	if (entity.has_component<SingleRigidBody>())
	{
		ComponentHandle<SingleRigidBody> rigid_body = entity.component<SingleRigidBody>();
		rigid_body->setShape(shape.get());
	}
	else
	{
		Logger::getInstance()->logError("No RigidBody on Entity #%d\n", entity.id().id());
	}
}

inline void PhysicsSystem::receive(const ComponentRemovedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;

	if (entity.has_component<SingleRigidBody>())
	{
		ComponentHandle<SingleRigidBody> rigid_body = entity.component<SingleRigidBody>();
		rigid_body->setShape(nullptr);
	}
}


#endif //PHYSICS_SYSTEM_HPP
