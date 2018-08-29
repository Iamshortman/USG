#include "VehicleController.hpp"

#include "Common/Transforms.hpp"

void VehicleSystem::update(EntityManager& es, EventManager& events, TimeDelta delta_time)
{
}

void VehicleSystem::configure(entityx::EventManager& event_manager)
{
	event_manager.subscribe<ComponentAddedEvent<Seat>>(*this);
	event_manager.subscribe<ComponentRemovedEvent<Seat>>(*this);
}

void VehicleSystem::receive(const ComponentAddedEvent<Seat>& event)
{
	Entity entity = event.entity;
	Entity parent = Transforms::getRootParentEntity(event.entity);

	if (parent.has_component<VehicleController>())
	{
		ComponentHandle<VehicleController> vehicle = parent.component<VehicleController>();
		ComponentHandle<Seat> seat = entity.component<Seat>();
		vehicle->seats.insert(seat.get());
	}
}

void VehicleSystem::receive(const ComponentRemovedEvent<Seat>& event)
{
	Entity entity = event.entity;
	Entity parent = Transforms::getRootParentEntity(event.entity);

	if (parent.has_component<VehicleController>())
	{
		ComponentHandle<VehicleController> vehicle = parent.component<VehicleController>();
		ComponentHandle<Seat> seat = entity.component<Seat>();
		vehicle->seats.erase(seat.get());
	}
}
