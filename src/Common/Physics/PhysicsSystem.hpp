#ifndef PHYSICS_SYSTEM_HPP

#include "Common/EntityX_Include.hpp"

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem>
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;


	//Events
	void configure(entityx::EventManager &event_manager);
	void receive(const ComponentAddedEvent<RigidBody> &event);

private:

};

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

inline void PhysicsSystem::update(EntityManager& es, EventManager& events, TimeDelta dt)
{

}

inline void PhysicsSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<RigidBody>>(*this);
}

inline void PhysicsSystem::receive(const ComponentAddedEvent<RigidBody>& event)
{
	printf("Event!!!!\n");
}

#endif //PHYSICS_SYSTEM_HPP
