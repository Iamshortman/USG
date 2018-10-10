#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
	ComponentHandle<CollisionShape> collision_shape_pattern;
	for (Entity entity : es.entities_with_components(collision_shape_pattern))
	{
		ComponentHandle<CollisionShape> shape = entity.component<CollisionShape>();

		Transform new_transform = Transforms::getParentRelativeTransform(entity);
		if (shape->last_relative_transform != new_transform)
		{
			shape->last_relative_transform = new_transform;
			shape->proxy->setLocalToBodyTransform(toRecTrans(new_transform));
		}
	}

	for (auto world : PhysicsWorldList::getInstance()->worlds)
	{
		world.second->update(delta_time);
	}
}

void PhysicsSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<RigidBody>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<RigidBody>>(*this);

	event_manager.subscribe<ComponentAddedEvent<CollisionShape>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<CollisionShape>>(*this);

	event_manager.subscribe<ComponentAddedEvent<WorldHost>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<WorldHost>>(*this);

	event_manager.subscribe<ComponentAddedEvent<World>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<World>>(*this);
}

void PhysicsSystem::receive(const ComponentAddedEvent<RigidBody>& event)
{
	Entity entity = event.entity;
	ComponentHandle<RigidBody> rigid_body = event.component;

	if (entity.has_component<World>())
	{
		Transform local_transform = Transforms::getLocalTransform(entity);

		PhysicsWorld* world = PhysicsWorldList::getInstance()->getPhysicsWorld(entity.component<World>()->world_id);
		if (world != nullptr)
		{
			rigid_body->createRigidBody(world, local_transform, entity.id());
		}
	}
}

void PhysicsSystem::receive(const ComponentRemovedEvent<RigidBody>& event)
{
	//Call Deconstructor
}

void PhysicsSystem::receive(const ComponentAddedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<CollisionShape> shape = event.component;

	shape->last_relative_transform = Transforms::getParentRelativeTransform(entity);
	Entity parent = Transforms::getRootParentEntity(entity);

	if (parent.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigidbody = parent.component<RigidBody>();
		rigidbody->addShape(shape.get(), entity.id());
	}
	else
	{
		Logger::getInstance()->logError("No Rigidbody on Parent: ID: %d\n", parent.id().id());
		entity.remove<CollisionShape>();
	}
}

void PhysicsSystem::receive(const ComponentRemovedEvent<CollisionShape>& event)
{
	Entity entity = event.entity;
	ComponentHandle<CollisionShape> shape = event.component;

	Entity parent = Transforms::getRootParentEntity(entity);

	if (parent.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigidbody = parent.component<RigidBody>();
		rigidbody->removeShape(shape.get());
	}
}

void PhysicsSystem::receive(const ComponentAddedEvent<WorldHost>& event)
{
	Entity entity = event.entity;
	ComponentHandle<WorldHost> world = event.component;

	if (!PhysicsWorldList::getInstance()->hasPhysicsWorld(world->world_id))
	{
		PhysicsWorldList::getInstance()->worlds[world->world_id] = new PhysicsWorld();
	}
}

void PhysicsSystem::receive(const ComponentRemovedEvent<WorldHost>& event)
{
	Entity entity = event.entity;
	ComponentHandle<WorldHost> world = event.component;

	if (PhysicsWorldList::getInstance()->hasPhysicsWorld(world->world_id))
	{
		delete PhysicsWorldList::getInstance()->worlds[world->world_id];
		PhysicsWorldList::getInstance()->worlds.erase(world->world_id);
	}
}

void PhysicsSystem::receive(const ComponentAddedEvent<World>& event)
{
	Entity entity = event.entity;

	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();

		if (event.component->world_id != INVALID_WORLD)
		{
			PhysicsWorld* world = PhysicsWorldList::getInstance()->getPhysicsWorld(event.component->world_id);
			if (world != nullptr)
			{
				rigid_body->createRigidBody(world, Transforms::getLocalTransform(entity), entity.id());
			}
		}
	}
}

void PhysicsSystem::receive(const ComponentRemovedEvent<World>& event)
{
	Entity entity = event.entity;

	if (entity.has_component<RigidBody>())
	{
		ComponentHandle<RigidBody> rigid_body = entity.component<RigidBody>();

		if (event.component->world_id != INVALID_WORLD)
		{
			rigid_body->destroyRigidBody();
		}
	}
}

PhysicsWorldList* PhysicsWorldList::instance = nullptr;
PhysicsWorldList* PhysicsWorldList::getInstance()
{
	if (PhysicsWorldList::instance == nullptr)
	{
		PhysicsWorldList::instance = new PhysicsWorldList();
	}

	return instance;
}

PhysicsWorldList::PhysicsWorldList()
{

}

PhysicsWorldList::~PhysicsWorldList()
{
}

bool PhysicsWorldList::hasPhysicsWorld(WorldId id)
{
	if (id == INVALID_WORLD)
	{
		return false;
	}

	return this->worlds.find(id) != this->worlds.end();
}

PhysicsWorld* PhysicsWorldList::getPhysicsWorld(WorldId id)
{
	if (this->hasPhysicsWorld(id))
	{
		return this->worlds[id];
	}

	return nullptr;
}
