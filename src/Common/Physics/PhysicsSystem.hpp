#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Common/Logger/Logger.hpp"

#include "Common/EntityX_Include.hpp"
#include "Common/Physics/RigidBody.hpp"
#include "Common/Transforms.hpp"

#include "Common/Physics/PhysicsWorld.hpp"

#include "Common/Physics/MuiltiCollisionShape.hpp"

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

	//Events
	void configure(entityx::EventManager &event_manager);

	void receive(const WorldChangeEvent& event);

	void receive(const ComponentAddedEvent<RigidBody> &event);
	void receive(const ComponentRemovedEvent<RigidBody>& event);

	void receive(const ComponentAddedEvent<CollisionShape>& event);
	void receive(const ComponentRemovedEvent<CollisionShape>& event);

	void receive(const ComponentAddedEvent<MuiltiCollisionShape>& event);
	void receive(const ComponentRemovedEvent<MuiltiCollisionShape>& event);

private:
	//PhysicsWorld* temp_world;
};

PhysicsSystem::PhysicsSystem()
{
	//this->temp_world = new PhysicsWorld();
	//this->temp_world->dynamicsWorld->setGravity(btVector3(0.0, -0.98, 0.0));
}

PhysicsSystem::~PhysicsSystem()
{
	//delete this->temp_world;
}

inline void PhysicsSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{
	ComponentHandle<WorldHost> world_host_pattern;
	for (Entity entity : es.entities_with_components(world_host_pattern))
	{
		ComponentHandle<WorldHost> world = entity.component<WorldHost>();
		world->physics_world->update(dt);
	}
}

inline void PhysicsSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<WorldChangeEvent>(*this);

	event_manager.subscribe<ComponentAddedEvent<RigidBody>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<RigidBody>>(*this);

	event_manager.subscribe<ComponentAddedEvent<CollisionShape>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<CollisionShape>>(*this);

	event_manager.subscribe<ComponentAddedEvent<MuiltiCollisionShape>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<MuiltiCollisionShape>>(*this);
}

inline void PhysicsSystem::receive(const WorldChangeEvent& event)
{
	Entity entity = event.entity;

	if (entity.has_component<RigidBody>())
	{
		if (entity.has_component<World>())
		{
			Entity old_world = WorldList::getInstance()->getWorldHost(event.old_world);
			old_world.component<WorldHost>()->physics_world->removeRigidBody(entity.component<RigidBody>().get());
		}

		Entity new_world = WorldList::getInstance()->getWorldHost(event.new_world);
		new_world.component<WorldHost>()->physics_world->removeRigidBody(entity.component<RigidBody>().get());
	}
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<RigidBody>& event)
{
	Entity entity = event.entity;
	ComponentHandle<RigidBody> rigid_body = event.component;

	if (entity.has_component<World>())
	{
		if (entity.has_component<Transform>())
		{
			rigid_body->setWorldTransform(*entity.component<Transform>());
		}

		if (entity.component<World>()->world_id != INVALID_WORLD)
		{
			Entity world_host = WorldList::getInstance()->getWorldHost(entity.component<World>()->world_id);
			ComponentHandle<WorldHost> world = world_host.component<WorldHost>();
			PhysicsWorld* physics_world = world->physics_world;

			physics_world->addRigidBody(rigid_body.get());
		}
	}
	else
	{
		Logger::getInstance()->logError("No World on Entity #%d\n", entity.id().id());
		entity.remove<RigidBody>();
	}
}

inline void PhysicsSystem::receive(const ComponentRemovedEvent<RigidBody>& event)
{
	Entity entity = event.entity;
	ComponentHandle<RigidBody> rigid_body = event.component;
	if (entity.has_component<World>())
	{
		if (entity.has_component<Transform>())
		{
			ComponentHandle<Transform> transform = entity.component<Transform>();
			transform.get()->setTransform(rigid_body->getWorldTransform());
		}

		if (entity.component<World>()->world_id != INVALID_WORLD)
		{
			Entity world_host = WorldList::getInstance()->getWorldHost(entity.component<World>()->world_id);
			ComponentHandle<WorldHost> world = world_host.component<WorldHost>();
			world->physics_world->removeRigidBody(rigid_body.get());
		}
	}
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<CollisionShape> shape = event.component;

	Entity parent = Transforms::getRootParentEntity(entity);
	if (parent.has_component<MuiltiCollisionShape>())
	{
		ComponentHandle<MuiltiCollisionShape> muilti_shape = parent.component<MuiltiCollisionShape>();
		Transform local_transform = Transforms::getParentRelativeTransform(entity);

		muilti_shape->compound_shape->addChildShape(toBtTransform(local_transform), (btCollisionShape*)shape->shape);
		return;
	}
	else if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();
		rigid_body->setCollisionShape((btCollisionShape*) shape->shape);
		return;
	}

	Logger::getInstance()->logError("No RigidBody on Entity #%d\n", entity.id().id());
	entity.remove<CollisionShape>();
}

inline void PhysicsSystem::receive(const ComponentRemovedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<CollisionShape> shape = event.component;

	Entity parent = Transforms::getRootParentEntity(entity);
	if (parent.has_component<MuiltiCollisionShape>())
	{
		ComponentHandle<MuiltiCollisionShape> muilti_shape = parent.component<MuiltiCollisionShape>();

		muilti_shape->compound_shape->removeChildShape((btCollisionShape*)shape->shape);
	}
	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();
		rigid_body->setCollisionShape(nullptr);
	}

	delete shape->shape;
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<MuiltiCollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<MuiltiCollisionShape> shape = event.component;

	shape->compound_shape = new btCompoundShape(true);

	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();
		rigid_body->setCollisionShape((btCollisionShape*)shape->compound_shape);
		return;
	}

	Logger::getInstance()->logError("No RigidBody on Entity #%d\n", entity.id().id());
	entity.remove<CollisionShape>();
}

inline void PhysicsSystem::receive(const ComponentRemovedEvent<MuiltiCollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<MuiltiCollisionShape> shape = event.component;

	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();
		rigid_body->setCollisionShape(nullptr);
	}

	delete shape->compound_shape;
}

#endif //PHYSICS_SYSTEM_HPP
